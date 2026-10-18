# RetroGames MVC

Aplicação didática baseada no template ASP.NET Core MVC em .NET 10. O catálogo e os pedidos ficam no mesmo processo; não há banco de dados nem Docker. Pedidos são mantidos em memória enquanto o aplicativo estiver em execução.

## Executar

1. Abra `RetroGamesMVC.csproj` na IDE com suporte ao SDK .NET 10.
2. Escolha o perfil **http** e inicie pelo botão Executar/Depurar.
3. Acesse `http://localhost:5193/`.

O perfil **https** do template usa `https://localhost:7016/` e `http://localhost:5193/`; ele pode exigir um certificado de desenvolvimento confiável. Os endereços estão em `Properties/launchSettings.json`.

Alternativa no terminal, a partir desta pasta: `dotnet run --launch-profile http`. Use `Ctrl+C` para encerrar.

## Fluxo

- `GET /` → rota convencional `{controller=Home}/{action=Index}/{id?}` → `HomeController.Index` → `Views/Home/Index.cshtml`.
- Formulário da View → `POST /Home/Criar` → `HomeController.Criar` → `PedidoService` → `JogoRepository` e `PedidoRepository` → `Views/Home/Index.cshtml`.
- `_ViewStart.cshtml` seleciona `_Layout.cshtml`; `_ViewImports.cshtml` habilita os Tag Helpers e importa os modelos.

O arquivo `Views/Home/Privacy.cshtml` e o tratamento de erro são os componentes demonstrativos do template. Os recursos em `wwwroot` são os arquivos estáticos e bibliotecas incluídos no template.
