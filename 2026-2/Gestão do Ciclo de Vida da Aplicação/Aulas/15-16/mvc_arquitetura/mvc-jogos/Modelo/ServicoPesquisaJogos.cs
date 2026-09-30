using Microsoft.Data.Sqlite;

namespace MvcJogos.Modelo;

public class ServicoPesquisaJogos
{
    private readonly string _stringConexao;

    public ServicoPesquisaJogos(string caminhoBanco)
    {
        _stringConexao = $"Data Source={caminhoBanco};Mode=ReadOnly";
    }

    // Retorna o primeiro jogo da franquia informada ou null, se não houver nenhum.
    // Aceita parte do nome e ignora maiúsculas/minúsculas: "zelda" encontra "The Legend of Zelda".
    public Jogo? PesquisaPorFranquia(string franquia)
    {
        // "using" fecha a conexão ao final do bloco (como o try-with-resources do Java)
        using var con = new SqliteConnection(_stringConexao);
        con.Open();

        using var cmd = con.CreateCommand();
        cmd.CommandText = """
            select id, titulo, franquia, plataforma, ano
              from jogos
             where franquia like '%' || @franquia || '%'
             order by id
             limit 1
            """;
        cmd.Parameters.AddWithValue("@franquia", franquia);

        using var rs = cmd.ExecuteReader();
        if (!rs.Read())           // posiciona na primeira linha; false = nenhum resultado
        {
            return null;
        }
        return new Jogo(
            rs.GetInt32(0), rs.GetString(1), rs.GetString(2), rs.GetString(3), rs.GetInt32(4));
    }
}
