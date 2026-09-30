using Pedidos.Repositories;
using Pedidos.Services;
using Pedidos.Models;
using Pedidos;

var builder = WebApplication.CreateBuilder(args);
builder.Services.AddControllersWithViews(); // habilita Controller e View Razor

builder.Services.AddSingleton<JogoRepository>();
builder.Services.AddSingleton<PedidoRepository>();
builder.Services.AddSingleton<PedidoService>();

var app = builder.Build();
app.MapControllers(); // conecta as rotas às ações dos Controllers
app.Run("http://localhost:8082");
