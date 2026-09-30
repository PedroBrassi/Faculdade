using Pedidos.ApiClients;
using Pedidos.Services;
using Pedidos.Models;
using Microsoft.AspNetCore.Mvc;

namespace Pedidos.Controllers;

// Controller: recebe a ação do navegador e escolhe a View e seus dados.
public class HomeController : Controller
{
    private readonly CatalogoClient _catalogo;
    private readonly PedidoService _pedidos;

    public HomeController(CatalogoClient catalogo, PedidoService pedidos)
    {
        _catalogo = catalogo;
        _pedidos = pedidos;
    }

    [HttpGet("/")]
    public async Task<IActionResult> Index()
    {
        var pagina = new PaginaModel { Pedidos = _pedidos.Listar() };
        try { pagina.Jogos = await _catalogo.ListarAsync(); }
        catch (HttpRequestException) { pagina.Mensagem = "Catálogo indisponível. Pedidos continua funcionando."; }
        catch (TaskCanceledException) { pagina.Mensagem = "O Catálogo demorou para responder."; }
        return View(pagina);
    }

    [HttpPost("/pedidos")]
    [ValidateAntiForgeryToken]
    public async Task<IActionResult> Criar(string cliente, int jogoId, int quantidade)
    {
        try
        {
            Pedido pedido = await _pedidos.CriarAsync(cliente, jogoId, quantidade);
            TempData["Mensagem"] = $"Pedido #{pedido.Id} criado.";
        }
        catch (ArgumentException erro) { TempData["Mensagem"] = erro.Message; }
        catch (HttpRequestException) { TempData["Mensagem"] = "Catálogo indisponível. Tente novamente."; }
        catch (TaskCanceledException) { TempData["Mensagem"] = "O Catálogo demorou para responder."; }
        return RedirectToAction(nameof(Index));
    }
}
