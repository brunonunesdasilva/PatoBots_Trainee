package Exerc1;

public class Main {
	public static void main(String[] args) throws InterruptedException {
        int porta = 1234;
        String host = "127.0.0.1";

        Thread servidorThread = new Thread(new Servidor(porta));
        servidorThread.start();

        // Aguarda o servidor iniciar
        Thread.sleep(1000);

        Thread clienteThread = new Thread(new Cliente(host, porta));
        clienteThread.start();

        // Aguarda o cliente terminar (por exemplo, ao digitar "sair")
        clienteThread.join();

        System.out.println("[Main] Aplicação finalizada.");
    }
}
