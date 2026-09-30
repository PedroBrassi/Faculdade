using Pedidos.ApiClients;
using Pedidos.Repositories;
using Pedidos.Services;
using Pedidos.Models;
using Pedidos;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddControllersWithViews(); // habilita Controller e View Razor

string urlCatalogo = Environment.GetEnvironmentVariable("CATALOGO_URL")
                     ?? "http://localhost:8081";
builder.Services.AddSingleton(new CatalogoClient(urlCatalogo));
builder.Services.AddSingleton<PedidoRepository>();
builder.Services.AddSingleton<PedidoService>();

var app = builder.Build();
app.MapControllers(); // conecta as rotas às ações dos Controllers
app.Run("http://0.0.0.0:8082");
