/**
* Exercício sobre o padrão de projeto Singleton
*/

class GameLogger {

    // TODO (1): impeça que outras classes criem instâncias de GameLogger
    // diretamente com "new GameLogger()". Dica: qual modificador de acesso
    // você pode usar no construtor para isso?


    // TODO (2): crie aqui um atributo estático e privado para guardar
    // a única instância de GameLogger que poderá existir.


    // TODO (3): crie um método público e estático chamado getInstance(),
    // que devolva a instância única de GameLogger — criando-a apenas na
    // primeira vez que for chamado, e reaproveitando-a nas chamadas seguintes.


    public void registrar(String evento) {
        // registra o evento do jogo na console, mas poderia ser em um arquivo
        System.out.println(evento);
    }

}


class Main {

  void teste() {
    // GameLogger log = new GameLogger(); // após a dica (1), esta linha deve
    // deixar de compilar. Descomente para testar (e comente de novo depois).
  }

  void sistemaDeCombate() {
    // TODO (4): o que deve ser alterado aqui?
    GameLogger log = new GameLogger();
    log.registrar("[Combate] Jogador derrotou um Goblin " + log);
  }

  void sistemaDeConquistas() {
    // TODO (5): faça a mesma alteração da dica (4) aqui
    GameLogger log = new GameLogger();
    log.registrar("[Conquistas] Jogador desbloqueou 'Primeiro Sangue' " + log);
  }

  void sistemaDeSave() {
    // TODO (6): faça a mesma alteração da dica (4) aqui
    GameLogger log = new GameLogger();
    log.registrar("[Save] Progresso salvo " + log);
  }

  public static void main(String[] args) {
    Main m = new Main();
    m.sistemaDeCombate();
    m.sistemaDeConquistas();
    m.sistemaDeSave();
    // TODO (7) [extra]: descomente a linha abaixo e rode o programa.
    // Compare o ID impresso aqui com os IDs impressos pelos três subsistemas.
    // System.out.println("Observe que os 3 subsistemas usaram o mesmo objeto, com ID: " + GameLogger.getInstance());
  }

}
