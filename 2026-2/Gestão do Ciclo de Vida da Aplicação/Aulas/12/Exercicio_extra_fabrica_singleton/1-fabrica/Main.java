/**
* Exercício sobre o padrão de projeto Fábrica
*/

// TODO (1): crie aqui uma interface chamada "IInimigo", com um método spawnar()
// Ela vai representar o "produto" que a fábrica saberá criar.


class GoblinInimigo {
    // TODO (2): faça esta classe implementar a interface IInimigo
    // (isso é necessário para que ela possa ser retornada pela fábrica)

    void spawnar() {
        System.out.println("Um Goblin surgiu no mapa!");
    }
}

// TODO (3): crie aqui uma classe OrcInimigo, também implementando IInimigo,
// cujo método spawnar() imprima "Um Orc surgiu no mapa!"
// (essa classe vai servir para você testar a troca de implementação no final)


// TODO (4): crie aqui uma classe InimigoFactory com um método fábrica
// estático criar(), que centralize a decisão de qual inimigo instanciar.
// Ex.: public static IInimigo criar() { ... return new GoblinInimigo(); }


public class Main {

  void ondaTutorial() {
    // TODO (5): O que deve ser alterado aqui?
    GoblinInimigo i = new GoblinInimigo();
    i.spawnar();
  }

  void ondaFase1() {
    // TODO (6): faça a mesma alteração da dica (5) aqui
    GoblinInimigo i = new GoblinInimigo();
    i.spawnar();
  }

  void arenaFinal() {
    // TODO (7): faça a mesma alteração da dica (5) aqui
    GoblinInimigo i = new GoblinInimigo();
    i.spawnar();
  }

  public static void main(String[] args) {
     Main m = new Main();
     m.ondaTutorial();
     m.ondaFase1();
     m.arenaFinal();
  }

}

// TODO (8) [extra]: depois de terminar, altere apenas o método criar() da
// InimigoFactory para retornar um OrcInimigo em vez de um GoblinInimigo, e
// rode o programa novamente. Repare que nenhuma outra classe precisou ser
// alterada.
