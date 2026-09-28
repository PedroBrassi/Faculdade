/**
* Exercício sobre o padrão de projeto Adaptador
*/

// Estas duas classes representam APIs de terceiros (de fabricantes diferentes)
// que não podem ser alteradas.

class ProjetorSamsung {
  public void turnOn() {
    System.out.println("Ligando projetor da Samsung");
  }
}

class ProjetorLG {
  public void enable(int timer) {
    System.out.println("Ligando projetor da LG em " + timer + " minutos");
  }
}

class SistemaControleProjetores {

  // Problema: como as duas classes de projetor têm métodos com nomes e
  // assinaturas diferentes (turnOn() x enable(int)), o sistema de controle
  // precisa conhecer cada marca individualmente e ter um método para cada
  // uma. Se surgir uma nova marca, será necessário alterar esta classe.

  void init(ProjetorSamsung projetor) {
    projetor.turnOn();
  }

  void init(ProjetorLG projetor) {
    projetor.enable(0);
  }

}

class Main {
  public static void main(String[] args) {
    SistemaControleProjetores scp = new SistemaControleProjetores();
    scp.init(new ProjetorSamsung());
    scp.init(new ProjetorLG());
  }
}

// TODO (1): crie uma interface "Projetor" com um método liga().
// TODO (2): crie uma classe AdaptadorProjetorSamsung que implemente Projetor,
// encapsulando um ProjetorSamsung e traduzindo liga() para turnOn().
// TODO (3): crie uma classe AdaptadorProjetorLG, de forma análoga, traduzindo
// liga() para enable(0).
// TODO (4) em SistemaControleProjetores: como simplificar esta classe para
// que ela dependa apenas da abstração Projetor?
// TODO (5) no Main: como instanciar e usar os adaptadores no lugar dos
// projetores concretos?
