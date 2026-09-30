using Catalogo.Models;
// Catálogo executa em um processo separado e responde por HTTP.
var builder = WebApplication.CreateBuilder(args);
var app = builder.Build();
var jogos = new List<Jogo>
{
    new Jogo(1, "Super Mario World", 249.90m),
    new Jogo(2, "Super Metroid", 299.90m),
    new Jogo(3, "Chrono Trigger", 549.90m)
};
app.MapGet("/jogos", () => jogos);
app.MapGet("/jogos/{id:int}", BuscarJogo);
app.Run("http://0.0.0.0:8081");

IResult BuscarJogo(int id)
{
    foreach (Jogo jogo in jogos)
    {
        if (jogo.Id == id)
            return Results.Ok(jogo);
    }
    return Results.NotFound();
}
