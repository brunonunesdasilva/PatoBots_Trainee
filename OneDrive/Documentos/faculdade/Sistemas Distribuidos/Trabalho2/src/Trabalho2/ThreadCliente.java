package Trabalho2;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class ThreadCliente implements Runnable {
	private Socket socket;
	private ObjectInputStream input;
	private ObjectOutputStream output;
	private String username;
	
	public ThreadCliente(Socket socket) {
		this.socket = socket;
	}
    
	@Override
	public void run() {
		try {
			output = new ObjectOutputStream(socket.getOutputStream());
			input = new ObjectInputStream(socket.getInputStream());
			
			boolean nomeValido = false;
			
			//loop para validar o nome do usuario
			while (!nomeValido) {
				username = (String) input.readObject();
				
				//verifica se o nome esta em uso
				if (servidor_TCP.clientes.containsKey(username)) {
					Mensagem erroNome = new Mensagem("SERVIDOR", null, "ERRO: Nome já em uso.");
					output.writeObject(erroNome);
					output.flush();
				} else {
					servidor_TCP.clientes.put(username, this);
					nomeValido = true;
					
					Mensagem sucesso = new Mensagem("SERVIDOR", username, "OK");
					output.writeObject(sucesso);
					output.flush();
				}
			}
            
			System.out.println(username + " conectou.");
			
			while (true) {
				Mensagem mensagem = (Mensagem) input.readObject();
				
				if (mensagem.getConteudo().startsWith("/usuarios")) {
					listaClientes(mensagem);
				}
				else if (mensagem.getDestinatario() == null) {
					broadcast(mensagem);
				}
				else {
					mensagemPrivada(mensagem);
				}
			}
		} catch (Exception e) {
			System.err.println(username + " se desconectou");
		} finally {
			// Garante que o nome só será limpo se o username chegou a ser definido
			if (username != null) {
				servidor_TCP.clientes.remove(username);
			}
			try {
				socket.close();
			} catch (IOException E) {
				
			}
		}
	}
	
	public void broadcast(Mensagem mensagem) {
		for (ThreadCliente cliente : servidor_TCP.clientes.values()) {
			cliente.enviarMensagem(mensagem);
		}
	}
    
	public void mensagemPrivada(Mensagem mensagem) {
		//verificacao para nao falar com si mesmo
		if (mensagem.getRemetente().equalsIgnoreCase(mensagem.getDestinatario())) {
	        Mensagem avisoAutoConversa = new Mensagem(
	            "SERVIDOR",
	            mensagem.getRemetente(),
	            "Não é possível enviar uma mensagem para você mesmo."
	        );
	        this.enviarMensagem(avisoAutoConversa);
	    }else {
	    	ThreadCliente destino = servidor_TCP.clientes.get(mensagem.getDestinatario());
	    	if (destino != null) { //verificacao para nao enviar a um destinatario inexistente
	    		destino.enviarMensagem(mensagem); 
	    		this.enviarMensagem(mensagem);
	    	}
	    	else {
	    		Mensagem avisoSistema = new Mensagem(
	    				"SERVIDOR",
	    				mensagem.getRemetente(), 
	    				"O usuário '" + mensagem.getDestinatario() + "' não está conectado ou não existe."
	    				);
	    		this.enviarMensagem(avisoSistema);
	    	}
	    }
		
	}
	
	public void listaClientes(Mensagem mensagem) {
		StringBuilder listaText = new StringBuilder();
		listaText.append("Usuários conectados neste momento:\n");
		
		//lista todos os clientes conectados
		for (String nomeCliente : servidor_TCP.clientes.keySet()) {
			listaText.append("- ").append(nomeCliente).append("\n");//mostra os usuarios online
		}
		
		listaText.append("Total: ").append(servidor_TCP.clientes.size()).append(" usuário(s) online.");
		
		Mensagem respostaLista = new Mensagem(
				"SERVIDOR", 
				mensagem.getRemetente(), 
				listaText.toString()
				);
		
		this.enviarMensagem(respostaLista); //envia a lista para o solicitante
	}
	
	public void enviarMensagem(Mensagem mensagem) {
		try {
			output.writeObject(mensagem);
			output.flush();
		} catch (IOException e) {
			System.out.println(username + " desconectado.");
			servidor_TCP.clientes.remove(username);
			try {
				socket.close();
			} catch (IOException ex) {
			}
		}
	}
}