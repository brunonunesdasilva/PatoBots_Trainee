package Trabalho2;

import java.io.Serializable;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class Mensagem implements Serializable {

    private String remetente;
    private String destinatario;
    private String conteudo;
    private LocalDateTime horario;

    // CONSTRUTOR
    public Mensagem(String remetente, String destinatario, String conteudo) {
    	this.remetente = remetente;
    	this.destinatario = destinatario;
    	this.conteudo = conteudo;
    	this.horario = LocalDateTime.now();
    }

    //Getters
    public String getRemetente() {
    	return remetente;
    }
    
    public String getDestinatario() {
    	return destinatario;
    }

    public String getConteudo() {
    	return conteudo;
    }

    public LocalDateTime getHorario() {
    	return horario;
    }
    
    @Override
    public String toString() {
    	DateTimeFormatter formatador = DateTimeFormatter.ofPattern("dd/MM/yyyy HH:mm:ss");
    	String horaFormatada = this.horario.format(formatador);

    	if (destinatario == null) {
    		return "[" + horaFormatada + "] " + remetente + ": " + conteudo;
    	}

    	return "[" + horaFormatada + "] " + remetente + " -> " + destinatario + ": " + conteudo;
    }
}