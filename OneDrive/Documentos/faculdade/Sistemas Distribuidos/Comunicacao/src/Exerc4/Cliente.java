package Exerc4;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

public class Cliente implements Runnable{
	
	private DatagramSocket socket;
	private InetAddress endereco;
	private int num1, num2;
	private String operacao;
	private byte[] buffer = new byte[1024];
	private final String host;
	private final int porta;
	
	public Cliente(String host, int porta, int num1, int num2, String operacao) {
		this.host = host;
		this.porta = porta;
		this.num1 = num1;
		this.num2 = num2;
		this.operacao = operacao;
	}
	
	@Override
	public void run() {
		try {
			//inicializar endereco
			endereco = InetAddress.getByName(host);
			socket = new DatagramSocket();
			
			String mensagem = operacao;
			byte[] msgBytes = mensagem.getBytes(StandardCharsets.UTF_8);
			
			ByteBuffer bufferEnvio = ByteBuffer.allocate(12 + msgBytes.length);
			bufferEnvio.putInt(num1);
			bufferEnvio.putInt(num2);
			bufferEnvio.putInt(msgBytes.length);
			bufferEnvio.put(msgBytes);
			
			byte[] msg = bufferEnvio.array();
			DatagramPacket dgEnvio = new DatagramPacket(msg, msg.length, endereco, porta);
			socket.send(dgEnvio);
			
			DatagramPacket dgRec = new DatagramPacket(buffer, buffer.length);
			socket.receive(dgRec);
			
			String msgRecebida = new String(dgRec.getData(), 0, dgRec.getLength(), StandardCharsets.UTF_8);			
			System.out.println("[Cliente] Resposta do Servidor: " + msgRecebida);
			socket.close();
			
		}catch(Exception e) {
			e.printStackTrace();
		}
	}
}
