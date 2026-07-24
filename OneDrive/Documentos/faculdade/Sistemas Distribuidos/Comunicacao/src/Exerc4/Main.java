package Exerc4;

import java.util.Scanner;

public class Main {
	public static void main(String[] args) throws InterruptedException {
        int porta = 1234;
        String host = "127.0.0.1";
        
        Scanner leitor = new Scanner(System.in);
        
        System.out.print("Digite o primeiro numero: ");
        int num1 = leitor.nextInt();
        
        System.out.print("Digite o segundo numero: ");
        int num2 = leitor.nextInt();
        
        leitor.nextLine();
        
        System.out.print("Digite a operacao (+, -, *, /): ");
        String operacao = leitor.nextLine();

        Thread servidorThread = new Thread(new Servidor(porta));
        servidorThread.start();

        // Aguarda o servidor iniciar
        Thread.sleep(1000);

        Thread clienteThread = new Thread(new Cliente(host, porta, num1, num2, operacao));
        clienteThread.start();

        // Aguarda o cliente terminar (por exemplo, ao digitar "sair")
        clienteThread.join();

        System.out.println("[Main] Aplicação finalizada.");
    }
}
