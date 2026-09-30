using System.Net;
using MvcJogos.Modelo;

namespace MvcJogos.Visao;

public class PaginaDadosJogo
{
// [V3]
    public string ExibeFormulario() => Pagina("""
        <h1>RetroGames MVC</h1>
        <h2>Pesquisa de Jogos</h2>
        <p>Informe a franquia do jogo:</p>
        <form action="/pesquisa" method="get">
          <input name="franquia" autofocus required>
          <button type="submit">Pesquisar</button>
        </form>
        <p>Franquias disponíveis: Mario, The Legend of Zelda e Metroid.</p>
        """);

    public string ExibeJogo(Jogo jogo)
    {
        string res = "<h4> Dados do Jogo Pesquisado </h4>";
        res += "<ul>";
        res += $"<li> Código: {jogo.Id} </li>";
        res += $"<li> Título: {Escapa(jogo.Titulo)} </li>";
        res += $"<li> Franquia: {Escapa(jogo.Franquia)} </li>";
        res += $"<li> Plataforma: {Escapa(jogo.Plataforma)} </li>";
        res += $"<li> Ano: {jogo.Ano} </li>";
        res += "</ul>";
        res += "<p><a href=\"/\">Nova pesquisa</a></p>";
        return Pagina(res);
    }

    public string ExibeJogoNaoEncontrado(string franquia)
    {
        string res = "<h4> Nenhum jogo encontrado </h4>";
        res += $"<p> Não há jogos da franquia \"{Escapa(franquia)}\". </p>";
        res += "<p><a href=\"/\">Nova pesquisa</a></p>";
        return Pagina(res);
    }

    private static string Pagina(string corpo) =>
        $"<!DOCTYPE html><html lang=\"pt-BR\"><head><meta charset=\"UTF-8\"><title>RetroGames MVC</title></head><body>{corpo}</body></html>";

    // Impede que um texto digitado pelo usuário seja interpretado como HTML
    // (por exemplo, <script>...</script>), o que caracterizaria um ataque XSS.
    private static string Escapa(string texto) => WebUtility.HtmlEncode(texto);
}

// GUIA DE LEITURA
// [V3] A visão gera HTML; não decide a busca nem acessa o SQLite.
