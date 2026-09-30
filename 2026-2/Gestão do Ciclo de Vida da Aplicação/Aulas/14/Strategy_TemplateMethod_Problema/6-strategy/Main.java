/**
* Exercício sobre o padrão de projeto Strategy
*/

import java.util.Arrays;

class MyList {

  private int[] elementos;
  private String tipoOrdenacao; // "bubble" ou "selection"

  public MyList(int[] elementos) {
    this.elementos = elementos;
    this.tipoOrdenacao = "bubble"; // default
  }

  public void setTipoOrdenacao(String tipo) {
    this.tipoOrdenacao = tipo;
  }

  // Problema: este método concentra a lógica de todos os algoritmos de
  // ordenação existentes, decidindo qual usar através de um if/else
  // baseado em uma String. Sempre que um novo algoritmo de ordenação for
  // necessário, será preciso alterar este método (violando o princípio
  // Aberto/Fechado), além de misturar, na mesma classe, a responsabilidade
  // de "ser uma lista" com a de "saber implementar vários algoritmos de
  // ordenação".
  public void sort() {
    if (tipoOrdenacao.equals("bubble")) {
      int n = elementos.length;
      int temp = 0;
      for (int i = 0; i < n; i++) {
        for (int j = 1; j < (n - i); j++) {
          if (elementos[j - 1] > elementos[j]) {
            temp = elementos[j - 1];
            elementos[j - 1] = elementos[j];
            elementos[j] = temp;
          }
        }
      }
    } else if (tipoOrdenacao.equals("selection")) {
      for (int i = 0; i < elementos.length - 1; i++) {
        int index = i;
        for (int j = i + 1; j < elementos.length; j++) {
          if (elementos[j] < elementos[index]) {
            index = j;
          }
        }
        int smallerNumber = elementos[index];
        elementos[index] = elementos[i];
        elementos[i] = smallerNumber;
      }
    }
  }

  public void print() {
    System.out.println(Arrays.toString(elementos));
  }
}

class Main {
  public static void main(String[] args) {
    System.out.println("Lista #1 foi ordenada com a estratégia default: BubbleSort");
    int[] elems1 = {3, 5, 2, 4, 1, 6};
    MyList list1 = new MyList(elems1);
    list1.sort();
    list1.print();

    System.out.println("\nLista #2 foi ordenada com uma outra estratégia: SelectionSort");
    int[] elems2 = {6, 5, 4, 3, 2, 1};
    MyList list2 = new MyList(elems2);
    list2.setTipoOrdenacao("selection");
    list2.sort();
    list2.print();
  }
}

// TODO (1): crie uma classe abstrata SortStrategy com um método abstrato
// sort(int[] elementos).
// TODO (2): crie as classes BubbleSortStrategy e SelectionSortStrategy, cada
// uma estendendo SortStrategy e implementando seu respectivo algoritmo (você
// pode aproveitar a lógica que já está no código-problema).
// TODO (3) em MyList: substitua o atributo tipoOrdenacao (String) por uma
// referência a um objeto SortStrategy; crie um método
// setSortStrategy(SortStrategy) para trocar a estratégia; e reescreva
// sort() para apenas delegar a chamada à estratégia atual.
// TODO (4) no Main: como configurar list2 para usar SelectionSortStrategy
// em vez de "selection"?
