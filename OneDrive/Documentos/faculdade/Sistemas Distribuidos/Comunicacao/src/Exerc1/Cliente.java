package Exerc1;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class Cliente implements Runnable{
	
	private DatagramSocket socket;
	private InetAddress endereco;
	private byte[] buffer = new byte[1024];
	private final String host;
	private final int porta;
	
	public Cliente(String host, int porta) {
		this.host = host;
		this.porta = porta;
	}
	
	@Override
	public void run() {
		try {
			//inicializar endereco
			endereco = InetAddress.getByName(host);
			socket = new DatagramSocket();
			
			String mensagem = "Ola isso e um teste";
			byte[] msg = mensagem.getBytes();
			DatagramPacket dgEnvio = new DatagramPacket(msg, msg.length, endereco, porta);
			socket.send(dgEnvio);
			
			DatagramPacket dgRec = new DatagramPacket(buffer, buffer.length);
			socket.receive(dgRec);
			
			String msgRecebida = new String(dgRec.getData()).trim();
			
			System.out.println("[Cliente] recebi " + msgRecebida+ " de "+ 
					dgRec.getAddress() + ":" + dgRec.getPort());
			socket.close();
			
		}catch(Exception e) {
			e.printStackTrace();
		}
	}
}
