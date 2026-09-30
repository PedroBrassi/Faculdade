/**
* Exercício sobre o padrão de projeto Template Method
*/

class FuncionarioCLT {

  protected double salario;

  public FuncionarioCLT(double salario) {
    this.salario = salario;
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

  // Problema: o "esqueleto" do cálculo (salário menos os três descontos)
  // está duplicado nesta classe e também em FuncionarioPJ, logo abaixo.
  // Se amanhã precisarmos mudar a fórmula geral (por exemplo, adicionar um
  // novo tipo de desconto para todos os funcionários), teremos que lembrar
  // de alterar essa lógica em todas as subclasses que a duplicam.
  public double calcSalarioLiquido() {
    double prev = calcDescontosPrevidencia();
    double saude = calcDescontosPlanoSaude();
    double outros = calcOutrosDescontos();
    return salario - prev - saude - outros;
  }
}

class FuncionarioPJ {

  protected double salario;

  public FuncionarioPJ(double salario) {
    this.salario = salario;
  }

  double calcDescontosPrevidencia() {
    return 0.0; // PJ não tem desconto de previdência na folha
  }

  double calcDescontosPlanoSaude() {
    return 50.0;
  }

  double calcOutrosDescontos() {
    return salario * 0.05; // imposto sobre nota fiscal, por exemplo
  }

  // Mesmo "esqueleto" de cálculo, duplicado
  public double calcSalarioLiquido() {
    double prev = calcDescontosPrevidencia();
    double saude = calcDescontosPlanoSaude();
    double outros = calcOutrosDescontos();
    return salario - prev - saude - outros;
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

// TODO (1): crie uma classe abstrata Funcionario, com o atributo salario e
// os três métodos de desconto declarados como abstratos
// (calcDescontosPrevidencia, calcDescontosPlanoSaude, calcOutrosDescontos).
// TODO (2): mova o método calcSalarioLiquido() (o "template method") para
// esta classe abstrata — ele deve continuar chamando os três métodos
// abstratos, mas agora existir em um único lugar.
// TODO (3): faça FuncionarioCLT e FuncionarioPJ estenderem Funcionario,
// removendo o atributo salario e o método calcSalarioLiquido() duplicados,
// e mantendo apenas as implementações específicas dos três métodos de
// desconto.
// TODO (4) no Main: precisa mudar alguma coisa na forma de usar
// FuncionarioCLT e FuncionarioPJ depois da refatoração?
