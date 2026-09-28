/**
* Exercício sobre o padrão de projeto Proxy
*/

class Book {
  String nome;
  public Book(String nome) {
    this.nome = nome;
  }
}

interface BookSearchInterface {
  Book getBook(String ISBN);
}

class BookSearch implements BookSearchInterface {

  public Book getBook(String ISBN) {
    System.out.println("Pesquisando no objeto base - ISBN " + ISBN);
    if (ISBN.equals("2")) {
       return new Book("GoF");
    }
    return null;
  }

}

class BookSearchProxy implements BookSearchInterface {

  private BookSearchInterface base;

  BookSearchProxy(BookSearchInterface base) {
    this.base = base;
  }

  public Book getBook(String ISBN) {
    Book book;
    System.out.println("Entrando no proxy - ISBN: " + ISBN);

    // A ideia aqui é que o Proxy conhece o livro que tem ISBN 1
    // Logo, ele nem precisa fazer a consulta ao objeto base
    if (ISBN.equals("1")) {
       System.out.println("Livro achado no proxy - ISBN: " + ISBN);
       book = new Book("ESM");
    }
    else {
      System.out.println("Livro não achado no proxy; repassando chamada para objeto base - ISBN: " + ISBN);
      book = base.getBook(ISBN);
    }
    System.out.println("Saindo do Proxy");
    return book;
  }

}

// TODO (1): crie uma nova classe (ex.: "BookSearchCacheProxy") que implemente
// BookSearchInterface, seguindo a mesma estrutura de BookSearchProxy — ou seja,
// ela também deve encapsular uma referência a um BookSearchInterface.


// TODO (2): adicione a essa classe um atributo para representar o cache.
// Para este exercício, você pode simular uma biblioteca de cache real usando
// um Map<String, Book>.


// TODO (3): implemente o método getBook(ISBN) desta classe. Antes de delegar
// a chamada ao objeto encapsulado, que verificação precisa ser feita?
// O que deve acontecer em cada um dos dois casos possíveis (livro já está
// no cache / livro ainda não está no cache)?


class Main {

  public static void main(String[] args) {
    BookSearch bs = new BookSearch();
    BookSearchProxy pbs = new BookSearchProxy(bs);

    // TODO (4): como usar a nova classe de cache junto com o que já existe
    // (bs e pbs), sem remover o comportamento atual? Teste pesquisando o
    // mesmo ISBN duas vezes seguidas e observe a diferença na saída do
    // console entre a primeira e a segunda chamada.

  }

}
