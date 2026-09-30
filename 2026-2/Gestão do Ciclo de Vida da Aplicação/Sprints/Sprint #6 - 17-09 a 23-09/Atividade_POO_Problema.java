/**
 * Atividade de revisão — Conceitos fundamentais de Orientação a Objetos
 *
 * O código abaixo representa, de forma propositalmente "malfeita", o sistema
 * de vendas de consoles de uma loja. Ele compila e funciona, mas ignora
 * praticamente todos os pilares da Orientação a Objetos.
 *
 * Evolua esse código em 5 etapas, seguindo os TODOs abaixo.
 */

class Console {
    public String nome;
    public String tipo;   // "nintendo", "playstation" ou "portatil"
    public double preco;
}

class Loja {

    public void venderConsole(Console console) {

        if (console.tipo.equals("nintendo")) {
            console.preco = console.preco * 1.10;
            System.out.println("Nintendo ligado.");
        } else if (console.tipo.equals("playstation")) {
            console.preco = console.preco * 1.20;
            System.out.println("Playstation ligado.");
        } else if (console.tipo.equals("portatil")) {
            console.preco = console.preco * 1.20;
            System.out.println("Playstation Portátil ligado.");
        }

        System.out.println(console.nome + " -> Preço final: R$ " + console.preco);
    }
}

public class Atividade_POO_Problema {

    public static void main(String[] args) {

        Console nintendo = new Console();
        nintendo.nome = "Nintendo Switch";
        nintendo.tipo = "nintendo";
        nintendo.preco = 2000;

        Console playstation = new Console();
        playstation.nome = "Playstation 5";
        playstation.tipo = "playstation";
        playstation.preco = 3000;

        Console portatil = new Console();
        portatil.nome = "Playstation Portátil";
        portatil.tipo = "portatil";
        portatil.preco = 2500;

        Loja loja = new Loja();
        loja.venderConsole(nintendo);
        loja.venderConsole(playstation);
        loja.venderConsole(portatil);
    }
}

// ===================== PARTE 1 — Encapsulamento =====================
// TODO (1.1): os atributos de Console estão públicos, permitindo que
// qualquer código altere nome/tipo/preco livremente (inclusive para
// valores inválidos, como um preco negativo). Torne os atributos privados.
// TODO (1.2): como os atributos agora são privados, crie métodos getters
// para nome, tipo e preco (pense: por que não criar também setters aqui?).

// ===================== PARTE 2 — Construtores =====================
// TODO (2.1): hoje um Console pode ser criado "pela metade" (por exemplo,
// esquecendo de definir o preco, que ficaria 0.0). Crie um construtor em
// Console que exija nome, tipo e preco, garantindo que todo objeto nasça
// em um estado válido.
// TODO (2.2): atualize o Main para criar os consoles usando o novo
// construtor, em vez de criar o objeto vazio e preencher os atributos um
// a um.

// ===================== PARTE 3 — Interface e Composição =====================
// TODO (3.1): crie uma interface IConsole com três métodos: void ligar(),
// double calcularPreco() e String getNome().
// TODO (3.2): crie uma classe DadosConsole com os atributos privados nome
// e precoBase, um construtor que os receba, e getters para os dois. Essa
// classe vai representar os dados que todo console tem em comum.
// TODO (3.3): crie as classes Nintendo e Playstation, cada uma implementando
// IConsole. Em vez de repetir nome/precoBase dentro delas, cada uma deve
// ter um atributo do tipo DadosConsole e delegar a ele as chamadas de
// getNome() e getPrecoBase() (isso é composição: "ter um" DadosConsole, em
// vez de repetir os mesmos atributos em cada classe).
// TODO (3.4): mova para Nintendo e Playstation a lógica que hoje está no
// if/else da Loja: a mensagem de ligar() e o percentual usado em
// calcularPreco() de cada uma (10% para Nintendo, 20% para Playstation).
// REFLEXÃO: por que usar uma classe separada (DadosConsole) em vez de
// colocar nome/precoBase diretamente dentro de Nintendo e Playstation?

// ===================== PARTE 4 — Herança (usada de forma apropriada) =====================
// TODO (4.1): a classe Console original também previa um console
// "portátil". Em vez de fazer PlaystationPortatil implementar IConsole do
// zero (repetindo tudo o que já existe em Playstation), crie
// PlaystationPortatil como uma subclasse: class PlaystationPortatil extends
// Playstation. (Dica: para isso funcionar, o atributo que guarda o
// DadosConsole em Playstation precisa deixar de ser private e virar
// protected, para que a subclasse consiga acessá-lo.)
// TODO (4.2): sobrescreva (@Override) o método ligar() em
// PlaystationPortatil com a mensagem específica dela.
// TODO (4.3): sobrescreva (@Override) também o método calcularPreco() em
// PlaystationPortatil, usando um percentual próprio (por exemplo, 15% em
// vez dos 20% herdados de Playstation) — afinal, a versão portátil pode ter
// uma composição de custos diferente da versão de mesa.
// REFLEXÃO: PlaystationPortatil está sobrescrevendo DOIS métodos herdados
// de Playstation. Isso é um problema? Compare com o caso de
// PlaystationPortatil.jogarDisco() que vocês viram no material de SOLID
// (Solucao_P1), que lançava UnsupportedOperationException. Qual a diferença
// entre sobrescrever um método para mudar SEU CÁLCULO e sobrescrever um
// método para RECUSAR fazer o que ele promete (violando o Princípio de
// Substituição de Liskov)?

// ===================== PARTE 5 — Polimorfismo e Extensibilidade (OCP) =====================
// TODO (5.1): reescreva venderConsole() da Loja para que ele receba um
// IConsole (não mais um Console) e apenas chame console.ligar() e
// console.calcularPreco(), sem nenhum if/else ou instanceof.
// TODO (5.2): crie em Loja um método venderVarios(List<IConsole> consoles)
// que chame venderConsole() para cada elemento de uma lista.
// TODO (5.3): crie em Loja um método
// calcularFaturamentoTotal(List<IConsole> consoles), que retorne a soma de
// calcularPreco() de todos os consoles da lista.
// TODO (5.4) [extensibilidade]: crie uma nova classe Xbox implementando
// IConsole (com seu próprio percentual, por exemplo 18%), SEM alterar
// nenhuma linha da classe Loja.
// TODO (5.5): no Main, monte uma List<IConsole> com os consoles já
// existentes, chame venderVarios() e calcularFaturamentoTotal(); em
// seguida, adicione um Xbox a essa lista e chame os dois métodos de novo,
// sem alterar Loja.
// DESAFIO FINAL: o que precisou mudar em Loja para o Xbox passar a
// funcionar? O que isso demonstra sobre o Princípio Aberto/Fechado (OCP)?