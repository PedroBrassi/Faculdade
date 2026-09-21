/**
* Exercício sobre o padrão de projeto Singleton
*/

class Logger {

    // TODO (1): impeça que outras classes criem instâncias de Logger
    // diretamente com "new Logger()". Dica: qual modificador de acesso
    // você pode usar no construtor para isso?


    // TODO (2): crie aqui um atributo estático e privado para guardar
    // a única instância de Logger que poderá existir.


    // TODO (3): crie um método público e estático chamado getInstance(),
    // que devolva a instância única de Logger — criando-a apenas na
    // primeira vez que for chamado, e reaproveitando-a nas chamadas seguintes.


    public void println(String msg) {
        // registra msg na console, mas poderia ser em um arquivo
        System.out.println(msg);
    }

}


class Main {

  void teste() {
    // Logger log = new Logger(); // após a dica (1), esta linha deve
    // deixar de compilar. Descomente para testar (e comente de novo depois).
  }

  void f() {
    // TODO (4): o que deve ser alterado aqui?
    Logger log = new Logger();
    log.println("Executando f " + log);
  }

  void g() {
    // TODO (5): faça a mesma alteração da dica (4) aqui
    Logger log = new Logger();
    log.println("Executando g " + log);
  }

  void h() {
    // TODO (6): faça a mesma alteração da dica (4) aqui
    Logger log = new Logger();
    log.println("Executando h " + log);
  }

  public static void main(String[] args) {
    Main m = new Main();
    m.f();
    m.g();
    m.h();
    // TODO (7) [extra]: descomente a linha abaixo e rode o programa.
    // Compare o ID impresso aqui com os IDs impressos por f(), g() e h().
    // System.out.println("Observe que as 3 chamadas executaram no mesmo objeto, com ID: " + Logger.getInstance());
  }

}
