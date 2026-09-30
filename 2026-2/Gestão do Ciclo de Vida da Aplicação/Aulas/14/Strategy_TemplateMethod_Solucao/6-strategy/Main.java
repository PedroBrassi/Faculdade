/**
* Exercício sobre o padrão de projeto Strategy
*/

import java.util.Arrays;

class MyList {

  private int[] elementos;

  private SortStrategy strategy;  // estratégia de ordenação

  public MyList(int[] elementos) {
    this.elementos = elementos;
    strategy = new BubbleSortStrategy(); // estratégia default: BubbleSort
  }

  public void setSortStrategy(SortStrategy strategy) {
    this.strategy = strategy;  // permite mudar estratégia de ordenação
  }

  public void sort() {
    strategy.sort(elementos);
  }

  public void print() {
    System.out.println(Arrays.toString(elementos));
  }
}

abstract class SortStrategy {
  abstract void sort(int[] elementos);
}

class BubbleSortStrategy extends SortStrategy {

  void sort(int[] elementos) {
    int n = elementos.length;
    int temp = 0;
    for (int i = 0; i < n; i++) {
      for (int j = 1; j < (n-i); j++) {
        if (elementos[j-1] > elementos[j]) {
           temp = elementos[j-1];
           elementos[j-1] = elementos[j];
           elementos[j] = temp;
        }
      }
    }
  }
}

class SelectionSortStrategy extends SortStrategy {

  void sort(int[] elementos) {
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
    list2.setSortStrategy(new SelectionSortStrategy());
    list2.sort();
    list2.print();

  }
}
