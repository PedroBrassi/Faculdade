/**
* Exercício sobre o padrão de projeto Observador
*/

class Jogador {
  private int pontuacao;

  // Problema: Jogador conhece diretamente a classe concreta
  // PainelDeConquistas e a instancia internamente. Isso impede que outros
  // tipos de "observadores" (por exemplo, um placar na tela, ou um logger
  // de eventos) sejam adicionados sem modificar esta classe, e também não
  // permite registrar múltiplos observadores dinamicamente em tempo de
  // execução.
  private PainelDeConquistas painel = new PainelDeConquistas();

  public int getPontuacao() {
    return pontuacao;
  }

  public void setPontuacao(int pontuacao) {
    this.pontuacao = pontuacao;
    painel.mostrar(this);
  }
}

class PainelDeConquistas {
  public void mostrar(Jogador j) {
    System.out.println("Pontuação atual: " + j.getPontuacao());
  }
}

public class Main {
  public static void main(String[] args) {
    Jogador j = new Jogador();
    j.setPontuacao(100);
  }
}

// TODO (1): crie uma classe Subject com uma lista de observadores e métodos
// addObserver, removeObserver e notifyObservers (que chama update(this) em
// cada observador).
// TODO (2): crie uma interface Observer com um método update(Subject s).
// TODO (3): faça Jogador estender Subject, remova a referência direta a
// PainelDeConquistas, e substitua a chamada direta por uma chamada a
// notifyObservers() dentro de setPontuacao().
// TODO (4): faça PainelDeConquistas implementar Observer, movendo a lógica
// de exibição para o método update(Subject s) (você vai precisar fazer um
// cast para Jogador dentro dele).
// TODO (5) no Main: como registrar o PainelDeConquistas como observador de
// Jogador antes de chamar setPontuacao()?
