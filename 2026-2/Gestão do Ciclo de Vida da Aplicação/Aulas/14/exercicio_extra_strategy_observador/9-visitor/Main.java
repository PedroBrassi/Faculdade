/**
* Exercício sobre o padrão de projeto Visitor
*/

import java.util.ArrayList;
import java.util.List;

abstract class ItemDoInventario {
  private String codigo;

  public ItemDoInventario(String codigo) {
    this.codigo = codigo;
  }

  public String getCodigo() {
    return codigo;
  }

  // Problema: a lógica de "descrever o item" foi implementada diretamente
  // em cada subclasse de ItemDoInventario. Isso funciona para esta única
  // operação, mas se amanhã precisarmos adicionar uma nova operação sobre a
  // hierarquia (por exemplo, calcular o valor de revenda, ou exportar para
  // JSON), será necessário abrir novamente ItemDoInventario e TODAS as suas
  // subclasses para adicionar mais um método a cada uma delas.
  abstract public void descrever();
}

class Arma extends ItemDoInventario {
  public Arma(String codigo) {
    super(codigo);
  }

  public void descrever() {
    System.out.println("Visitando uma Arma com código: " + getCodigo());
  }
}

class Pocao extends ItemDoInventario {
  public Pocao(String codigo) {
    super(codigo);
  }

  public void descrever() {
    System.out.println("Visitando uma Poção com código: " + getCodigo());
  }
}

public class Main {
  public static void main(String[] args) {
    List<ItemDoInventario> lista = new ArrayList<ItemDoInventario>();
    lista.add(new Arma("ARM-1020"));
    lista.add(new Pocao("POC-3456"));
    lista.add(new Arma("ARM-1234"));
    lista.add(new Pocao("POC-7923"));

    for (ItemDoInventario item : lista) {
      item.descrever();
    }
  }
}

// TODO (1): crie uma interface VisitanteDeItem com um método visita(Arma a)
// e um método visita(Pocao p) (sobrecarga por tipo).
// TODO (2) em ItemDoInventario: substitua o método abstrato descrever() por
// um método abstrato accept(VisitanteDeItem v).
// TODO (3) em Arma e Pocao: implemente accept(VisitanteDeItem v) chamando
// v.visita(this) (repare que o tipo de "this" já é conhecido em tempo de
// compilação em cada subclasse).
// TODO (4): crie uma classe VisitanteDeDescricao que implemente
// VisitanteDeItem, movendo para ela a lógica que hoje está nos métodos
// descrever() de Arma e Pocao.
// TODO (5) no Main: como usar VisitanteDeDescricao para visitar cada item
// da lista, chamando accept() em vez de descrever() diretamente?
