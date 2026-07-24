package Trabalho2;


import java.net.Socket;
import java.util.Scanner;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;

public class Cliente_TCP {
	private int porta;
	private String host;
	
	public Cliente_TCP(String host, int porta) {
		this.host = host;
		this.porta = porta;
	}

	public static void main(String[] args) {
		try {
			Socket socket = new Socket("localhost", 1234);

			ObjectOutputStream output = new ObjectOutputStream(socket.getOutputStream()); //saida dos objetos
			ObjectInputStream input = new ObjectInputStream(socket.getInputStream());
			Scanner teclado = new Scanner(System.in);
			boolean conectado = false; //variavel para verificar a conectividade do cliente
			String nome = "";

			//cadastro do username
			while (!conectado) {
			    System.out.print("Digite seu nome: ");//mensagem do servidor ou print ?
			    nome = teclado.nextLine();

			    // Envia o nome para o servidor testar
			    output.writeObject(nome);
			    output.flush();

			    // Recebe a resposta para verificar se o nome esta disponivel
			    Mensagem respostaServidor = (Mensagem) input.readObject();

			    if (respostaServidor.getConteudo().equals("OK")) {
			    	conectado = true; // sai do loop
			    } else {
			    	// Erro para nome ja conectado
			    	System.out.println(respostaServidor.getConteudo() + " Por favor, escolha outro.");
			    	// O loop continua e pede o nome novamente na próxima volta!
			    }
			}

			// thread para o cliente receber mensagem
			Thread receber = new Thread(new ThreadEscuta(socket, input));
			receber.start();

			System.out.println("Conectado a sala de bate papo!");

			// loop para enviar mensagem 
			while (true) {

				System.out.print("> ");
				String texto = teclado.nextLine();
				Mensagem mensagem; //sempre declarada vazia para cada mensagem ser diferente

				// mensagem privada
				if(texto.startsWith("/privado")) {

					String[] partes = texto.split(" ", 3);
					String destino = partes[1];
					String conteudo = partes[2];

					mensagem = new Mensagem(nome, destino, conteudo);

				} else {
					// broadcast-/usuarios
					mensagem = new Mensagem(nome,null, texto);
				}
				
				//prepara e envia a mensagem
				output.writeObject(mensagem);
				output.flush();
			}

		} catch (Exception e) {

			System.out.println("Erro ao conectar.");
		}
	}
}