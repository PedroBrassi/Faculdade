/**
* Exercício sobre o padrão de projeto Adaptador
*/

// Estas duas classes representam APIs de terceiros (de fabricantes diferentes)
// que não podem ser alteradas.

class ControleXboxAPI {
  public void ligar() {
    System.out.println("Ligando controle Xbox via Bluetooth");
  }
}

class ControleDualSenseAPI {
  public void parear(int canal) {
    System.out.println("Pareando controle DualSense no canal " + canal);
  }
}

class GerenciadorDeControles {

  // Problema: como as duas classes de controle têm métodos com nomes e
  // assinaturas diferentes (ligar() x parear(int)), o sistema de input
  // precisa conhecer cada marca individualmente e ter um método para cada
  // uma. Se surgir uma nova marca (ex.: Joy-Con), será necessário alterar
  // esta classe.

  void iniciar(ControleXboxAPI controle) {
    controle.ligar();
  }

  void iniciar(ControleDualSenseAPI controle) {
    controle.parear(0);
  }

}

class Main {
  public static void main(String[] args) {
    GerenciadorDeControles gc = new GerenciadorDeControles();
    gc.iniciar(new ControleXboxAPI());
    gc.iniciar(new ControleDualSenseAPI());
  }
}

// TODO (1): crie uma interface "IControle" com um método conectar().
// TODO (2): crie uma classe AdaptadorControleXbox que implemente IControle,
// encapsulando um ControleXboxAPI e traduzindo conectar() para ligar().
// TODO (3): crie uma classe AdaptadorControleDualSense, de forma análoga,
// traduzindo conectar() para parear(0).
// TODO (4) em GerenciadorDeControles: como simplificar esta classe para
// que ela dependa apenas da abstração IControle?
// TODO (5) no Main: como instanciar e usar os adaptadores no lugar dos
// controles concretos?
