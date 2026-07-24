package Exerc4;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

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
	        System.out.println("[Servidor] Calculadora pronta na porta " + porta);

	        while (true) {
	            DatagramPacket dgRec = new DatagramPacket(buffer, buffer.length);
	            socket.receive(dgRec);

	            ByteBuffer wrapped = ByteBuffer.wrap(dgRec.getData());
	            
	            int n1 = wrapped.getInt();
	            int n2 = wrapped.getInt();
	            int tamString = wrapped.getInt();
	            
	            byte[] stringBytes = new byte[tamString];
	            wrapped.get(stringBytes);
	            String op = new String(stringBytes, StandardCharsets.UTF_8).trim();

	            double resultado = 0;
	            String respostaStr;

	            switch (op.toLowerCase()) {
	                case "+": resultado = n1 + n2; break;
	                case "-": resultado = n1 - n2; break;
	                case "*": resultado = n1 * n2; break;
	                case "/": 
	                    if (n2 != 0) resultado = (double) n1 / n2;
	                    else { respostaStr = "Erro: Divisão por zero"; break; }
	                default: 
	                    respostaStr = "Operação inválida";
	            }
	            
	            respostaStr = "Resultado: " + resultado;
	            System.out.println("[Servidor] Calculou: " + n1 + " " + op + " " + n2);

	            byte[] respostaBytes = respostaStr.getBytes();
	            DatagramPacket dgEnvio = new DatagramPacket(respostaBytes, respostaBytes.length, 
	                                                        dgRec.getAddress(), dgRec.getPort());
	            socket.send(dgEnvio);
	        }
	    } catch (Exception e) {
	        e.printStackTrace();
	    } finally {
	        if (socket != null && !socket.isClosed()) socket.close();
	    }
	}
}
