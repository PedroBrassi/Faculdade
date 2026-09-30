/**
* Exercício sobre o padrão de projeto Template Method
*/

class PersonagemGuerreiro {

  protected double danoBase;

  public PersonagemGuerreiro(double danoBase) {
    this.danoBase = danoBase;
  }

  double calcReducaoPorArmadura() {
    return danoBase * 0.1;
  }

  double calcReducaoPorResistencia() {
    return 10.0;
  }

  double calcReducaoPorDistancia() {
    return 5.0;
  }

  // Problema: o "esqueleto" do cálculo (dano base menos as três reduções)
  // está duplicado nesta classe e também em PersonagemMago, logo abaixo.
  // Se amanhã precisarmos mudar a fórmula geral (por exemplo, adicionar um
  // novo tipo de redução para todos os personagens), teremos que lembrar
  // de alterar essa lógica em todas as subclasses que a duplicam.
  public double calcDanoFinal() {
    double armadura = calcReducaoPorArmadura();
    double resistencia = calcReducaoPorResistencia();
    double distancia = calcReducaoPorDistancia();
    return danoBase - armadura - resistencia - distancia;
  }
}

class PersonagemMago {

  protected double danoBase;

  public PersonagemMago(double danoBase) {
    this.danoBase = danoBase;
  }

  double calcReducaoPorArmadura() {
    return 0.0; // magia ignora armadura física
  }

  double calcReducaoPorResistencia() {
    return danoBase * 0.15; // resistência mágica do alvo
  }

  double calcReducaoPorDistancia() {
    return 0.0; // feitiços não perdem dano com a distância
  }

  // Mesmo "esqueleto" de cálculo, duplicado
  public double calcDanoFinal() {
    double armadura = calcReducaoPorArmadura();
    double resistencia = calcReducaoPorResistencia();
    double distancia = calcReducaoPorDistancia();
    return danoBase - armadura - resistencia - distancia;
  }
}

class Main {
  public static void main(String[] args) {
    PersonagemGuerreiro guerreiro = new PersonagemGuerreiro(100);
    System.out.println("Dano final (Guerreiro): " + guerreiro.calcDanoFinal());

    PersonagemMago mago = new PersonagemMago(100);
    System.out.println("Dano final (Mago): " + mago.calcDanoFinal());
  }
}

// TODO (1): crie uma classe abstrata Personagem, com o atributo danoBase e
// os três métodos de redução declarados como abstratos
// (calcReducaoPorArmadura, calcReducaoPorResistencia, calcReducaoPorDistancia).
// TODO (2): mova o método calcDanoFinal() (o "template method") para esta
// classe abstrata — ele deve continuar chamando os três métodos abstratos,
// mas agora existir em um único lugar.
// TODO (3): faça PersonagemGuerreiro e PersonagemMago estenderem
// Personagem, removendo o atributo danoBase e o método calcDanoFinal()
// duplicados, e mantendo apenas as implementações específicas dos três
// métodos de redução.
// TODO (4) no Main: precisa mudar alguma coisa na forma de usar
// PersonagemGuerreiro e PersonagemMago depois da refatoração?
