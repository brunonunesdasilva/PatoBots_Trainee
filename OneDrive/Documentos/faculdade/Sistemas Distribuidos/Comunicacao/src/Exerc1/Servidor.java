package Exerc1;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;

public class Servidor implements Runnable{
	private DatagramSocket socket;
	private byte[] buffer = new byte[1024];
	private String nome;
	private final int porta;
	
	public Servidor(int porta) {
		this.porta = porta;
	}
	
	@Override
	public void run() {
		try {
			socket = new DatagramSocket(porta);
			while(true) {
				DatagramPacket dgRec = new DatagramPacket(buffer, buffer.length);
				socket.receive(dgRec);
				
				String mensagem = new String(dgRec.getData()).trim();
				System.out.println("[Servidor] recebi " + mensagem + " de "+ 
						dgRec.getAddress() + ":" + dgRec.getPort());
				
				byte[] resposta = "ola cliente".getBytes();
				
				DatagramPacket dgEnvio = new DatagramPacket(resposta, resposta.length, dgRec.getAddress(), dgRec.getPort());
				socket.send(dgEnvio);
				
				if (mensagem.equalsIgnoreCase("sair")) {
                    System.out.println("[Servidor] Encerrando...");
                    break;
                }
			}
		}catch (Exception e){
			e.printStackTrace();
		}finally {
			if(socket != null && !socket.isClosed() ) {
				socket.close();
			}
		}
	}
	
}
