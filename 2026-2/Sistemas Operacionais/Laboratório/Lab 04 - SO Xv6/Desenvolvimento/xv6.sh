#!/usr/bin/env bash
# =============================================================
#  xv6.sh - abre o ambiente xv6 (Linux)               versao 2
#
#  COMO USAR (sempre assim, sem sudo):
#      (terminal DENTRO da pasta xv6, onde quer que ela esteja)
#      bash xv6.sh         # abre o ambiente
#      bash xv6.sh         # (de novo, em outra janela) 2o terminal p/ o gdb
#      bash xv6.sh --diagnostico   # verifica o que pode estar errado
#
#  TUDO fica NESTA pasta (a mesma do xv6.sh):
#      xv6.sh, so-xv6-2024.tar, kit-lab01/, ..., xv6-labs-2024/ (seu codigo)
#  Dentro do container, esta pasta aparece como /lab.
# =============================================================
IMAGE="${XV6_IMAGE:-so-xv6:2024}"
NAME="xv6-lab"
HERE="$(cd "$(dirname "$0")" && pwd)"
DIR="${XV6_DIR:-$HERE}"
ME="${USER:-$(id -un)}"
DIAG=0; [ "$1" = "--diagnostico" ] && DIAG=1

ok()   { echo -e "  \e[1;32m[OK]\e[0m $*"; }
erro() { echo -e "  \e[1;31m[ERRO]\e[0m $*"; }
info() { echo -e "\e[1;36m>> $*\e[0m"; }

# 0) Nao usar sudo
if [ "$(id -u)" = "0" ] && [ -n "$SUDO_USER" ]; then
    erro "nao use sudo com este script. Rode apenas:  bash xv6.sh"
    echo "       Se o Docker nao abrir sem sudo, peca a TI:  sudo usermod -aG docker $SUDO_USER"
    exit 1
fi

# 1) Docker ou Podman?
if docker info >/dev/null 2>&1; then
    ENGINE=docker; USEROPT=(--user "$(id -u):$(id -g)")
    # Docker "rootless" ou Docker Desktop: o root do container JA E o aluno.
    # Nesses casos --user atrapalha (da "Permission denied" ao escrever na pasta).
    if docker info -f '{{.SecurityOptions}} {{.OperatingSystem}}' 2>/dev/null | grep -qiE 'rootless|Docker Desktop'; then
        USEROPT=()
        [ "$1" = "--diagnostico" ] && echo -e "  \e[1;32m[OK]\e[0m Docker rootless/Desktop detectado (rodando sem --user)"
    fi
elif command -v podman >/dev/null 2>&1; then
    ENGINE=podman; USEROPT=(--userns=keep-id)
else
    erro "o Docker nao esta acessivel para o seu usuario ($ME)."
    if id -nG "$ME" 2>/dev/null | grep -qw docker; then
        echo "       Voce ja esta no grupo docker: faca logout/login ou rode:  newgrp docker"
    else
        echo "       Peca a TI:  sudo usermod -aG docker $ME   (e depois logout/login)"
    fi
    exit 1
fi
[ $DIAG = 1 ] && ok "Docker/Podman acessivel ($ENGINE)"

# 2) A pasta de trabalho pode ser escrita?
mkdir -p "$DIR" 2>/dev/null
if ! ( touch "$DIR/.teste-escrita" && rm -f "$DIR/.teste-escrita" ) 2>/dev/null; then
    erro "voce nao tem permissao de escrita na pasta:  $DIR"
    echo "       Dono atual: $(stat -c '%U' "$DIR" 2>/dev/null)   (deveria ser: $ME)"
    echo "       Solucao 1: copie a pasta xv6 inteira para uma pasta sua (ex.: Documentos) e rode de la:"
    echo "                  cp -r \"$HERE\" ~/Documentos/xv6  &&  cd ~/Documentos/xv6  &&  bash xv6.sh"
    echo "       Solucao 2 (se a pasta foi criada com sudo):  sudo chown -R $ME: \"$DIR\""
    exit 1
fi
if [ -e "$DIR/xv6-labs-2024" ] && ! [ -w "$DIR/xv6-labs-2024" ]; then
    erro "a pasta do codigo $DIR/xv6-labs-2024 nao e sua (foi criada com sudo?)."
    echo "       Solucao:  sudo chown -R $ME: \"$DIR/xv6-labs-2024\"   ou apague-a e rode de novo."
    exit 1
fi
[ $DIAG = 1 ] && ok "Pasta de trabalho com permissao de escrita: $DIR"

# 3) SELinux (Fedora/RHEL): a pasta precisa do rotulo :z para o container usa-la
VOLOPT=""
if command -v getenforce >/dev/null 2>&1 && [ "$(getenforce 2>/dev/null)" = "Enforcing" ]; then
    VOLOPT=":z"
    [ $DIAG = 1 ] && ok "SELinux ativo: usando rotulo :z na pasta"
fi

# 4) Imagem presente? Se nao, carrega o .tar desta pasta
if ! $ENGINE image inspect "$IMAGE" >/dev/null 2>&1; then
    TAR=$(ls "$HERE"/so-xv6*.tar 2>/dev/null | head -1)
    if [ -n "$TAR" ]; then
        info "Carregando a imagem de $TAR (so na primeira vez, ~1 min)..."
        $ENGINE load -i "$TAR" || { erro "falha ao carregar $TAR"; exit 1; }
    else
        erro "imagem $IMAGE nao encontrada neste computador."
        echo "       Coloque o arquivo so-xv6-2024.tar nesta pasta ($HERE) e rode de novo."
        exit 1
    fi
fi
[ $DIAG = 1 ] && ok "Imagem $IMAGE presente"
if [ $DIAG = 1 ]; then
    echo "  [i] usuario: $(id -u):$(id -g) ($ME)   sistema de arquivos da pasta: $(stat -f -c %T "$DIR" 2>/dev/null)"
    echo "  [i] docker: $($ENGINE info -f '{{.OperatingSystem}} | {{.SecurityOptions}}' 2>/dev/null)"
fi

# 4b) O container consegue escrever na pasta? Testa e escolhe o modo que funciona.
#     Ordem: 1) como o seu usuario  2) com rotulo SELinux  3) pasta local (/var/tmp, /tmp)
#     4) so por ultimo: como root no container, devolvendo os arquivos ao seu usuario no fim.
pode_escrever() { $ENGINE run --rm "$@" -v "$DIR":/lab$VOLOPT "$IMAGE" sh -c 'touch /lab/.t && rm -f /lab/.t' >/dev/null 2>&1; }
[ -n "$XV6_TEST_UID" ] && USEROPT=(--user "$XV6_TEST_UID")   # apenas para testes
USERMODE=("${USEROPT[@]}")
ORIG="$DIR"; MODO=""
tenta_pasta() {   # $1 = pasta; tenta como usuario e com :z
    DIR="$1"
    VOLOPT=""; pode_escrever "${USERMODE[@]}" && return 0
    VOLOPT=":z"; pode_escrever "${USERMODE[@]}" && return 0
    VOLOPT=""; return 1
}
if tenta_pasta "$ORIG"; then MODO="normal"
else
    for LOCAL in "/var/tmp/xv6-$ME" "/tmp/xv6-$ME"; do
        mkdir -p "$LOCAL" 2>/dev/null && chmod 700 "$LOCAL" 2>/dev/null || continue
        if tenta_pasta "$LOCAL"; then MODO="pasta local $LOCAL"; break; fi
    done
fi
if [ -z "$MODO" ]; then
    # ultimo recurso: root no container (Docker rootless/Desktop ou casos especiais)
    for P in "$ORIG" "/var/tmp/xv6-$ME" "/tmp/xv6-$ME"; do
        [ -d "$P" ] || continue
        DIR="$P"; VOLOPT=""
        if pode_escrever; then USEROPT=(); MODO="root no container ($P)"; break; fi
    done
fi
if [ -z "$MODO" ]; then
    erro "o container nao consegue escrever em $ORIG, /var/tmp nem /tmp."
    echo "       Rode  bash xv6.sh --diagnostico  e mostre o resultado ao professor."
    exit 1
fi
if [ "$DIR" != "$ORIG" ]; then
    cp -rn "$ORIG"/kit-lab* "$DIR"/ 2>/dev/null
    echo -e "\e[1;33m!! AVISO: o container nao consegue escrever na pasta $ORIG"
    echo -e "   (comum em home de rede). Usando a pasta LOCAL: $DIR"
    echo -e "   Seu codigo ficara em: $DIR/xv6-labs-2024"
    echo -e "   Ao terminar a aula, copie essa pasta para o seu pendrive/nuvem.\e[0m"
fi
[ $DIAG = 1 ] && ok "Modo de execucao: $MODO"

# 5) Diagnostico: testa o container de verdade e para por aqui
if [ $DIAG = 1 ]; then
    $ENGINE run --rm "${USEROPT[@]}" -v "$DIR":/lab$VOLOPT "$IMAGE" bash -c '
      touch /lab/.t 2>/dev/null && rm -f /lab/.t && echo -e "  \e[1;32m[OK]\e[0m O container consegue escrever na pasta (/lab)" \
        || echo -e "  \e[1;31m[ERRO]\e[0m O container NAO consegue escrever em /lab (permissao/SELinux)"
      git -C "$XV6_MIRROR" rev-parse --verify -q util >/dev/null && echo -e "  \e[1;32m[OK]\e[0m Codigo do xv6 presente na imagem" \
        || echo -e "  \e[1;31m[ERRO]\e[0m A imagem nao tem o codigo do xv6 (reconstrua a imagem)"
      qemu-system-riscv64 --version | head -1 | sed "s/^/  [OK] /"'
    exit 0
fi

# 6) Container ja aberto? Entao abre um segundo terminal nele (uso com gdb)
if [ -n "$($ENGINE ps -q -f name=^/?${NAME}$)" ]; then
    info "Abrindo um terminal extra no xv6 ja em execucao..."
    exec $ENGINE exec -it -e XV6_QUIET=1 -e PATH="/tmp/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" -w /lab/xv6-labs-2024 "$NAME" bash
fi

# 7) Abre o container. Mesmo se algo falhar la dentro, o terminal continua aberto.
# Docker rootless/Desktop: root do container ja e o seu usuario -> nao mexer na posse dos arquivos
NOCHOWN=""; $ENGINE info -f '{{.SecurityOptions}} {{.OperatingSystem}}' 2>/dev/null | grep -qiE 'rootless|Docker Desktop' && NOCHOWN=1
info "Pasta de trabalho: $DIR   (no container: /lab)"
exec $ENGINE run --rm -it --name "$NAME" --hostname xv6 \
    "${USEROPT[@]}" -e HOST_UID="$(id -u)" -e HOST_GID="$(id -g)" -e XV6_NOCHOWN="$NOCHOWN" \
    -v "$DIR":/lab$VOLOPT \
    "$IMAGE" \
    bash -c '# evita o aviso "groups: cannot find name for group ID" (grupo do host nao existe no container)
             mkdir -p /tmp/bin && printf "#!/bin/sh\necho aluno\n" > /tmp/bin/groups && chmod +x /tmp/bin/groups
             export PATH=/tmp/bin:$PATH
             xv6-preparar || echo -e "\e[1;31m!! Nao consegui criar /lab/xv6-labs-2024. Saia (exit) e rode: bash xv6.sh --diagnostico\e[0m"
             devolve() { [ "$(id -u)" = 0 ] && [ -z "$XV6_NOCHOWN" ] && chown -R "$HOST_UID:$HOST_GID" /lab/xv6-labs-2024 /lab/kit-lab* 2>/dev/null; }
             devolve
             cd /lab/xv6-labs-2024 2>/dev/null || cd /lab
             bash
             devolve'
