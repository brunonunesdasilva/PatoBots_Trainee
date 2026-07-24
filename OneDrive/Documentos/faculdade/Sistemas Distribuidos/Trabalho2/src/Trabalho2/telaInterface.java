package Trabalho2;

import javax.swing.*;
import java.awt.*;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;

public class telaInterface extends JFrame {

    private JTextArea areaChat;
    private JTextField campoMensagem;
    private JButton botaoEnviar;
    private JButton botaoSair;

    private Socket socket;

    private ObjectOutputStream output;
    private ObjectInputStream input;

    private String nome;

    public telaInterface(String nome) {

        this.nome = nome;
        setTitle("Chat TCP - " + nome);
        setSize(500, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BorderLayout());

        // ÁREA CHAT
        areaChat = new JTextArea();
        areaChat.setEditable(false);

        JScrollPane scroll = new JScrollPane(areaChat);

        add(scroll, BorderLayout.CENTER);

        // PAINEL INFERIOR
        JPanel painel = new JPanel();
        painel.setLayout(new BorderLayout());

        campoMensagem = new JTextField();

        botaoEnviar = new JButton("Enviar");
        botaoSair = new JButton("Desconectar");

        painel.add(campoMensagem, BorderLayout.CENTER);

        painel.add(botaoEnviar, BorderLayout.EAST);
        painel.add(botaoSair, BorderLayout.WEST);

        add(painel, BorderLayout.SOUTH);

        conectar();

        // BOTÃO ENVIAR
        botaoEnviar.addActionListener(e -> {

            enviarMensagem();
        });

        // ENTER
        campoMensagem.addActionListener(e -> {

            enviarMensagem();
        });

        //BOTÃO DESCONCETAR
        botaoSair.addActionListener(e -> {

            desconectar();
        });

        setVisible(true);
    }

    private void conectar() {

        try {

            socket = new Socket("localhost", 1234);

            output = new ObjectOutputStream(socket.getOutputStream());

            input = new ObjectInputStream(socket.getInputStream());

            // ENVIA NOME
            output.writeObject(nome);
            output.flush();

            // THREAD RECEBENDO
            new Thread(() -> {

                try {

                    while (true) {

                        Mensagem msg = (Mensagem) input.readObject();

                        areaChat.append(msg.toString() + "\n");
                    }

                } catch (Exception e) {

                    areaChat.append("Conexão encerrada.\n");
                }

            }).start();

        } catch (Exception e) {

            JOptionPane.showMessageDialog(this, "Erro ao conectar.");
        }
    }

    private void enviarMensagem() {

        try {

            String texto = campoMensagem.getText();

            if(texto.isEmpty()) {
                return;
            }

            Mensagem msg;

            // PRIVADO
            if(texto.startsWith("/privado")) {

                String[] partes = texto.split(" ", 3);

                msg = new Mensagem(
                        nome,
                        partes[1],
                        partes[2]);

            } else {

                msg = new Mensagem(
                        nome,
                        null,
                        texto);
            }

            output.writeObject(msg);
            output.flush();

            campoMensagem.setText("");

        } catch (Exception e) {

            areaChat.append("Erro ao enviar.\n");
        }
    }

    private void desconectar() {

        try {

            areaChat.append("Desconectando...\n");

            output.close();

            input.close();

            socket.close();

            dispose();

        } catch (Exception e) {

            areaChat.append(
                    "Erro ao desconectar.\n");
        }
    }

    public static void main(String[] args) {

        String nome =
                JOptionPane.showInputDialog(
                        "Digite seu nome:");

        new telaInterface(nome);
    }
}