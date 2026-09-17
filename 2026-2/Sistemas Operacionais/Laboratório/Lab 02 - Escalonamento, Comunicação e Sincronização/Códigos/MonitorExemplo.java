class BufferMonitor {
    private int item;
    private boolean cheio = false;

    public synchronized void inserir(int valor) throws InterruptedException {
        while (cheio) {
            wait();
        }
        item = valor;
        cheio = true;
        System.out.println("Produzi: " + valor);
        notifyAll();
    }

    public synchronized int retirar() throws InterruptedException {
        while (!cheio) {
            wait();
        }
        int valor = item;
        cheio = false;
        System.out.println("Consumido: " + valor);
        notifyAll();
        return valor;
    }
}

public class MonitorExemplo {
    public static void main(String[] args) {
        BufferMonitor buffer = new BufferMonitor();

        Thread produtor = new Thread(() -> {
            try {
                for (int i = 1; i <= 5; i++) {
                    buffer.inserir(i);
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });

        Thread consumidor = new Thread(() -> {
            try {
                for (int i = 1; i <= 5; i++) {
                    buffer.retirar();
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });

        produtor.start();
        consumidor.start();
    }
}