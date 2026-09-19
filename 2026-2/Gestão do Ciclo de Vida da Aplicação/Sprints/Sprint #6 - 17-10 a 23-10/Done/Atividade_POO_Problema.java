/**
 * Atividade de revisão — Conceitos fundamentais de Orientação a Objetos
 *
 * Versão evoluída, aplicando encapsulamento, construtores, interface,
 * composição, herança e polimorfismo (OCP) sobre o código original.
 */

import java.util.ArrayList;
import java.util.List;

// ===================== PARTE 1 e 2 — Encapsulamento e Construtores =====================
class Console {
    private String nome;
    private String tipo;
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

// ===================== PARTE 4 — Herança =====================
class PlaystationPortatil extends Playstation {

    public PlaystationPortatil(String nome, double precoBase) {
        super(nome, precoBase);
    }

    @Override
    public void ligar() {
        System.out.println("Playstation Portátil ligado.");
    }

    @Override
    public double calcularPreco() {
        return dados.getPrecoBase() * 1.15;
    }
}

// ===================== PARTE 5 — Polimorfismo e Extensibilidade =====================
class Xbox implements IConsole {
    protected DadosConsole dados;

    public Xbox(String nome, double precoBase) {
        this.dados = new DadosConsole(nome, precoBase);
    }

    @Override
    public void ligar() {
        System.out.println("Xbox ligado.");
    }

    @Override
    public double calcularPreco() {
        return dados.getPrecoBase() * 1.18;
    }

    @Override
    public String getNome() {
        return dados.getNome();
    }
}

class Loja {

    public void venderConsole(IConsole console) {
        console.ligar();
        System.out.println(console.getNome() + " -> Preço final: R$ " + console.calcularPreco());
    }

    public void venderVarios(List<IConsole> consoles) {
        for (IConsole console : consoles) {
            venderConsole(console);
        }
    }

    public double calcularFaturamentoTotal(List<IConsole> consoles) {
        double total = 0;
        for (IConsole console : consoles) {
            total += console.calcularPreco();
        }
        return total;
    }
}

public class Atividade_POO_Problema {

    public static void main(String[] args) {

        Loja loja = new Loja();

        List<IConsole> consoles = new ArrayList<>();
        consoles.add(new Nintendo("Nintendo Switch", 2000));
        consoles.add(new Playstation("Playstation 5", 3000));
        consoles.add(new PlaystationPortatil("Playstation Portátil", 2500));

        loja.venderVarios(consoles);
        System.out.println("Faturamento total: R$ " + loja.calcularFaturamentoTotal(consoles));

        // Extensibilidade (OCP): adicionamos um Xbox sem alterar nada em Loja.
        consoles.add(new Xbox("Xbox Series X", 2800));

        loja.venderVarios(consoles);
        System.out.println("Faturamento total: R$ " + loja.calcularFaturamentoTotal(consoles));
    }
}
