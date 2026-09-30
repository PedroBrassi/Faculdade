using MvcJogos.Modelo;
using MvcJogos.Visao;

namespace MvcJogos.Controladores;

public class ControladorPesquisaJogos
{
    private readonly ServicoPesquisaJogos _pesq;
    private readonly PaginaDadosJogo _pagina;

    public ControladorPesquisaJogos(ServicoPesquisaJogos pesq, PaginaDadosJogo pagina)
    {
        _pesq = pesq;
        _pagina = pagina;
    }

    // Registra as rotas que este controlador atende
    public void RegistrarRotas(WebApplication app)
    {
// [V1]
        app.MapGet("/", () => Html(_pagina.ExibeFormulario(), 200));

// [V2]
        app.MapGet("/pesquisa", (string? franquia) =>
        {
            if (string.IsNullOrWhiteSpace(franquia))
            {
                return Results.Redirect("/");      // nada foi digitado: volta ao formulário
            }

            Jogo? jogo = _pesq.PesquisaPorFranquia(franquia.Trim());   // Modelo
            if (jogo is null)
            {
                return Html(_pagina.ExibeJogoNaoEncontrado(franquia), 404);   // Visão
            }
            return Html(_pagina.ExibeJogo(jogo), 200);                        // Visão
        });
    }

    private static IResult Html(string conteudo, int status) =>
        Results.Content(conteudo, "text/html; charset=utf-8", statusCode: status);
}

// GUIA DE LEITURA
// [V1] O formulário também passa pelo controlador didático.
// [V2] A rota recebe a entrada, consulta o modelo e escolhe a visão.
