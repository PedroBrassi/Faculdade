using Pedidos.Models;
using System.Net.Http.Json;

namespace Pedidos.ApiClients;

// Única classe de Pedidos que conhece o endereço do outro serviço.
public class CatalogoClient
{
    private readonly HttpClient _http;

    public CatalogoClient(string url)
    {
        _http = new HttpClient { BaseAddress = new Uri(url), Timeout = TimeSpan.FromSeconds(5) };
    }

    public async Task<List<Jogo>> ListarAsync()
    {
        List<Jogo>? jogos = await _http.GetFromJsonAsync<List<Jogo>>("/jogos");
        return jogos ?? new List<Jogo>();
    }

    public async Task<Jogo?> BuscarAsync(int id)
    {
        using var resposta = await _http.GetAsync($"/jogos/{id}");
        if (resposta.StatusCode == System.Net.HttpStatusCode.NotFound) return null;
        resposta.EnsureSuccessStatusCode();
        return await resposta.Content.ReadFromJsonAsync<Jogo>();
    }
}
