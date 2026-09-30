using MvcJogos.Controladores;
using MvcJogos.Modelo;
using MvcJogos.Visao;

var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();

string caminhoBanco = Path.Combine(AppContext.BaseDirectory, "db", "jogos.db");

new ControladorPesquisaJogos(
        new ServicoPesquisaJogos(caminhoBanco),   // classe do Modelo
        new PaginaDadosJogo()                     // classe da Visão
    ).RegistrarRotas(app);

Console.WriteLine("Aplicação rodando em http://localhost:5000");
app.Run("http://localhost:5000");
