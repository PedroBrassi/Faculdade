using Pedidos.Models;
namespace Pedidos.Repositories;

public class JogoRepository
{
    private readonly List<Jogo> _jogos = new List<Jogo>()
    {
        new Jogo(1, "Super Mario World", 249.90m),
        new Jogo(2, "Super Metroid", 299.90m),
        new Jogo(3, "Chrono Trigger", 549.90m)
    };
    public List<Jogo> Listar()
    {
        return new List<Jogo>(_jogos);
    }

    public Jogo? Buscar(int id)
    {
        foreach (Jogo jogo in _jogos)
        {
            if (jogo.Id == id)
                return jogo;
        }
        return null; // ? no tipo de retorno indica que o jogo pode não existir.
    }
}
