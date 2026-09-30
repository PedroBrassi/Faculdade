# Exemplo de MVC: RetroGames

Exemplo mínimo de uma aplicação C#/.NET que segue uma Arquitetura MVC, com motivação didática apenas.

- **Modelo:** pasta `Modelo` (`Jogo` e `ServicoPesquisaJogos`, que consulta o banco SQLite)
- **Visão:** pasta `Visao` (`PaginaDadosJogo`, que gera o formulário e o HTML da resposta)
- **Controlador:** pasta `Controladores` (`ControladorPesquisaJogos`, que recebe as requisições `/` e `/pesquisa`, chama o Modelo na pesquisa e escolhe a Visão)
- `Program.cs` só monta as peças e liga o servidor web.

## Como Executar?

Requisito: [.NET SDK 8](https://dotnet.microsoft.com/download) (no Ubuntu: `sudo apt install dotnet-sdk-8.0`).

Digite no diretório do projeto:

```dotnet run```

E depois entre em um browser e digite: `http://localhost:5000`

Franquias disponíveis: **Mario**, **The Legend of Zelda** e **Metroid**
(pode digitar só parte do nome, como `zelda`).

## Diagrama de Sequência

```mermaid
sequenceDiagram
    autonumber
    actor User as Navegador
    participant Ctrl as ControladorPesquisaJogos (Controlador)
    participant Svc as ServicoPesquisaJogos (Modelo)
    participant DB as SQLite
    participant View as PaginaDadosJogo (Visao)

    User->>Ctrl: GET /
    Ctrl->>View: ExibeFormulario()
    View-->>Ctrl: HTML (formulário)
    Ctrl-->>User: 200 OK + HTML

    rect rgb(245,245,245)
      note over User,Ctrl: Consulta por franquia
      User->>Ctrl: GET /pesquisa?franquia=Nome
      Ctrl->>Svc: PesquisaPorFranquia(franquia)
      Svc->>DB: new SqliteConnection("Data Source=db/jogos.db")
      Svc->>DB: SELECT ... FROM jogos WHERE franquia LIKE ?
      DB-->>Svc: linha (id, titulo, franquia, plataforma, ano)
      alt jogo encontrado
        Svc-->>Ctrl: new Jogo(id, titulo, franquia, plataforma, ano)
        Ctrl->>View: ExibeJogo(jogo)
        View-->>Ctrl: HTML (lista com dados do jogo)
        Ctrl-->>User: 200 OK + HTML
      else nenhum jogo
        Svc-->>Ctrl: null
        Ctrl->>View: ExibeJogoNaoEncontrado(franquia)
        View-->>Ctrl: HTML (mensagem)
        Ctrl-->>User: 404 Not Found + HTML
      end
    end
```

## Para ir além

O ASP.NET Core tem um framework MVC completo (classes `Controller` e páginas
Razor `.cshtml`), que segue a "vertente Web" de MVC discutida na seção 7.3 do
livro. Este exemplo monta as três partes à mão, de propósito, para que o papel
de cada uma fique visível.
