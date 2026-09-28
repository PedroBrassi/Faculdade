/**
* Exercício sobre o padrão de projeto Proxy
*/

class Asset {
  String nome;
  public Asset(String nome) {
    this.nome = nome;
  }
}

interface AssetLoaderInterface {
  Asset getAsset(String assetId);
}

class AssetLoader implements AssetLoaderInterface {

  public Asset getAsset(String assetId) {
    System.out.println("Carregando do disco/rede - asset ID " + assetId);
    if (assetId.equals("2")) {
       return new Asset("Modelo3D_Dragao");
    }
    return null;
  }

}

class AssetLoaderProxy implements AssetLoaderInterface {

  private AssetLoaderInterface base;

  AssetLoaderProxy(AssetLoaderInterface base) {
    this.base = base;
  }

  public Asset getAsset(String assetId) {
    Asset asset;
    System.out.println("Entrando no proxy - asset ID: " + assetId);

    // A ideia aqui é que o Proxy conhece o asset de ID 1 (embutido no jogo)
    // Logo, ele nem precisa fazer a consulta ao objeto base
    if (assetId.equals("1")) {
       System.out.println("Asset achado no proxy - ID: " + assetId);
       asset = new Asset("Textura_ChaoPadrao");
    }
    else {
      System.out.println("Asset não achado no proxy; repassando chamada para objeto base - ID: " + assetId);
      asset = base.getAsset(assetId);
    }
    System.out.println("Saindo do Proxy");
    return asset;
  }

}

// TODO (1): crie uma nova classe (ex.: "AssetCacheProxy") que implemente
// AssetLoaderInterface, seguindo a mesma estrutura de AssetLoaderProxy — ou
// seja, ela também deve encapsular uma referência a um AssetLoaderInterface.


// TODO (2): adicione a essa classe um atributo para representar o cache.
// Para este exercício, você pode simular uma biblioteca de cache real usando
// um Map<String, Asset>.


// TODO (3): implemente o método getAsset(assetId) desta classe. Antes de
// delegar a chamada ao objeto encapsulado, que verificação precisa ser
// feita? O que deve acontecer em cada um dos dois casos possíveis (asset já
// está no cache / asset ainda não está no cache)?


class Main {

  public static void main(String[] args) {
    AssetLoader al = new AssetLoader();
    AssetLoaderProxy pal = new AssetLoaderProxy(al);

    // TODO (4): como usar a nova classe de cache junto com o que já existe
    // (al e pal), sem remover o comportamento atual? Teste carregando o
    // mesmo asset ID duas vezes seguidas e observe a diferença na saída do
    // console entre a primeira e a segunda chamada.

  }

}
