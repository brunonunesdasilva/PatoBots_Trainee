package Trabalho2;

import java.io.IOException;
import java.net.Socket;
import java.io.ObjectInputStream;


public class ThreadEscuta implements Runnable{
	private Socket socket;
	private ObjectInputStream input;
	
	public ThreadEscuta(Socket socket, ObjectInputStream input) {
		this.socket = socket;
		this.input = input;
	}
	@Override
	public void run() {
		try {
			while (true) {//loop de escuta infinita, sempre que algum usuario enviar uma mensagem ela sera printada no terminal
				Mensagem mensagem = (Mensagem) input.readObject(); //thread dorme ate ter um write object seja enviado para ser lido

				System.out.println("\n" + mensagem);
			}
		} catch (IOException e) {
			System.out.println("Conexão com o servidor perdida");
		} catch (ClassNotFoundException e) {
			System.out.println("A classe Mensagem não existe");
		}
	}
}