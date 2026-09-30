/**
* Exercício sobre o padrão de projeto Template Method
*/

abstract class Funcionario {

   protected double salario;

   public Funcionario(double salario) {
     this.salario = salario;
   }

   abstract double calcDescontosPrevidencia();
   abstract double calcDescontosPlanoSaude();
   abstract double calcOutrosDescontos();

   /**
   * Template Method: define o esqueleto de um algoritmo
   */
   public double calcSalarioLiquido() {
     double prev = calcDescontosPrevidencia();
     double saude = calcDescontosPlanoSaude();
     double outros = calcOutrosDescontos();
     return salario - prev - saude - outros;
   }
}

class FuncionarioCLT extends Funcionario {

  public FuncionarioCLT(double salario) {
     super(salario);
  }

  double calcDescontosPrevidencia() {
     return salario * 0.1;
  }

  double calcDescontosPlanoSaude() {
     return 100.0;
  }

  double calcOutrosDescontos() {
    return 20.0;
  }

}

class FuncionarioPJ extends Funcionario {

  public FuncionarioPJ(double salario) {
     super(salario);
  }

  double calcDescontosPrevidencia() {
     return 0.0;
  }

  double calcDescontosPlanoSaude() {
     return 50.0;
  }

  double calcOutrosDescontos() {
    return salario * 0.05;
  }

}

class Main {

  public static void main(String[] args) {
    FuncionarioCLT func1 = new FuncionarioCLT(1000);
    System.out.println("Salário Líquido (CLT): " + func1.calcSalarioLiquido());

    FuncionarioPJ func2 = new FuncionarioPJ(1000);
    System.out.println("Salário Líquido (PJ): " + func2.calcSalarioLiquido());
  }

}
