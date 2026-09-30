/**
* Exercício sobre o padrão de projeto Strategy
*/

class Personagem {

  private String nome;
  private String tipoControle; // "joystick", "movimento" ou "voz"

  public Personagem(String nome) {
    this.nome = nome;
    this.tipoControle = "joystick"; // default
  }

  public void setTipoControle(String tipo) {
    this.tipoControle = tipo;
  }

  // Problema: este método concentra a lógica de todas as formas de
  // controle existentes, decidindo qual usar através de um if/else baseado
  // em uma String. Sempre que uma nova forma de controle for necessária,
  // será preciso alterar este método (violando o princípio Aberto/Fechado),
  // além de misturar, na mesma classe, a responsabilidade de "ser um
  // personagem" com a de "saber implementar vários tipos de controle".
  public void mover(String direcao) {
    if (tipoControle.equals("joystick")) {
      System.out.println(nome + " se move para " + direcao + " (comando via joystick).");
    } else if (tipoControle.equals("movimento")) {
      System.out.println(nome + " se move para " + direcao + " (detectado pelo sensor de movimento).");
    } else if (tipoControle.equals("voz")) {
      System.out.println(nome + " se move para " + direcao + " (comando de voz reconhecido).");
    }
  }
}

class Main {
  public static void main(String[] args) {
    System.out.println("Personagem #1 é controlado com a estratégia default: Joystick");
    Personagem p1 = new Personagem("Kael");
    p1.mover("frente");

    System.out.println("\nPersonagem #2 é controlado com uma outra estratégia: Comando de voz");
    Personagem p2 = new Personagem("Mira");
    p2.setTipoControle("voz");
    p2.mover("esquerda");
  }
}

// TODO (1): crie uma classe abstrata EstrategiaControle com um método
// abstrato mover(String nome, String direcao).
// TODO (2): crie as classes ControleJoystick, ControleMovimento e
// ControlePorVoz, cada uma estendendo EstrategiaControle e implementando
// sua mensagem específica (você pode aproveitar as mensagens que já estão
// no código-problema).
// TODO (3) em Personagem: substitua o atributo tipoControle (String) por
// uma referência a um objeto EstrategiaControle; crie um método
// setEstrategiaControle(EstrategiaControle) para trocar a estratégia; e
// reescreva mover(String direcao) para apenas delegar a chamada à
// estratégia atual.
// TODO (4) no Main: como configurar p2 para usar ControlePorVoz em vez de
// "voz"?
