/**
 * Atividade de revisão — Conceitos fundamentais de Orientação a Objetos
 *
 * O código abaixo representa, de forma propositalmente "malfeita", o sistema
 * de vendas de consoles de uma loja. Ele compila e funciona, mas ignora
 * praticamente todos os pilares da Orientação a Objetos.
 *
 * Evolua esse código em 5 etapas, seguindo os TODOs abaixo.
 */


// ===================== PARTE 1 e 2 — Encapsulamento e Construtores =====================
class Console {
    private String nome;
    private String tipo;   // "nintendo", "playstation" ou "portatil"
    private double preco;

    public Console(String nome, String tipo, double preco) {
        this.nome = nome;
        this.tipo = tipo;
        this.preco = preco;
    }

    public String getNome() {
        return nome;
    }

    public String getTipo() {
        return tipo;
    }

    public double getPreco() {
        return preco;
    }
}

// ===================== PARTE 3 — Interface e Composição =====================
interface IConsole {
    void ligar();
    double calcularPreco();
    String getNome();
}

class DadosConsole {
    private String nome;
    private double precoBase;

    public DadosConsole(String nome, double precoBase) {
        this.nome = nome;
        this.precoBase = precoBase;
    }

    public String getNome() {
        return nome;
    }

    public double getPrecoBase() {
        return precoBase;
    }
}

class Nintendo implements IConsole {
    protected DadosConsole dados;

    public Nintendo(String nome, double precoBase) {
        this.dados = new DadosConsole(nome, precoBase);
    }

    @Override
    public void ligar() {
        System.out.println("Nintendo ligado.");
    }

    @Override
    public double calcularPreco() {
        return dados.getPrecoBase() * 1.10;
    }

    @Override
    public String getNome() {
        return dados.getNome();
    }
}

class Playstation implements IConsole {
    protected DadosConsole dados;

    public Playstation(String nome, double precoBase) {
        this.dados = new DadosConsole(nome, precoBase);
    }

    @Override
    public void ligar() {
        System.out.println("Playstation ligado.");
    }

    @Override
    public double calcularPreco() {
        return dados.getPrecoBase() * 1.20;
    }

    @Override
    public String getNome() {
        return dados.getNome();
    }
}

class Loja {

    public void venderConsole(IConsole console) {

        console.ligar();
        double precoFinal = console.calcularPreco();

        System.out.println(console.getNome() + " -> Preço final: R$ " + precoFinal);
    }
}

public class Atividade_POO_Problema {

    public static void main(String[] args) {

        IConsole nintendo = new Nintendo("Nintendo Switch", 2000);
        IConsole playstation = new Playstation("Playstation 5", 3000);
        IConsole portatil = new PlaystationPortatil("Playstation Portátil", 2500);

        Loja loja = new Loja();
        loja.venderConsole(nintendo);
        loja.venderConsole(playstation);
        loja.venderConsole(portatil);
    }
}

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