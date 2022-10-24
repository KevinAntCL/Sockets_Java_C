// KEVIN ANTONIO CHÁVEZ LARA - 185380
import java.io.*;
import java.net.*;

class MovieClient {

	public static void main(String[] args) throws IOException
	{
        // CREAR LA OPCION DE ARGUMENTOS PARA HOOTNAME Y PORT
        if (args.length < 2)
        { 
            System.out.println("Modo de uso: MovieClient IPAddress portNumber");
            System.exit(1);
        }
        int port = Integer.parseInt(args[1]); 
        String hostName = args[0]; 
        // Open your connection to a server, at port 1234
        Socket s1 = new Socket(hostName,port);
		// Cliente 
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
		System.out.print("Nombre de la pelicula a buscar: ");
        String nombrePelicula = in.readLine(); // Lee el nombre de la pelicula.
        System.out.print("\nOpciones de busqueda:\n- Genre            1\n- Lead Studio      2\n- Audience score   3\n- Profitability    4\n- Rotten Tomatoes  5\n- Worldwide Gross  6\n- Year             7\nElija la opcion: ");
        String opcionConsulta = in.readLine(); // Lee la opcion/opcode.
        // CREAMOS EL MENSAJE DE PROTOCOLO CON OPCODE + STRING
        String mensajeProtocolo = new String(opcionConsulta);
        mensajeProtocolo = mensajeProtocolo + "," + nombrePelicula;
		// Mensaje del protocolo se envia al server utilizando un socket:
        OutputStream s1out = s1.getOutputStream();
        DataOutputStream dos = new DataOutputStream (s1out);
        dos.write(mensajeProtocolo.getBytes("utf-8"));
		// Server recibe el mensaje y lo procesa, para luego mandar el resultado:

        InputStream s1In = s1.getInputStream();
        DataInputStream dis = new DataInputStream(s1In);

        byte[] buffer = new byte[255];
        dis.read(buffer);
        String resultadoProtocolo= new String (buffer);
        String[] mensajeSplit = resultadoProtocolo.split(",");

        System.out.println("El resultado de la busqueda fue: ");
        if (Integer.parseInt(mensajeSplit[0]) == 1)
        {
            System.out.print("EXITOSO: ");
            System.out.println(mensajeSplit[1]);
        }
        else
        {
            System.out.print("FALLIDO: No se encontró una pelicula con el nombre especificado.");
        }

        /////////////////////////////
        dis.close();
        s1In.close();
        s1.close();
        dos.close();
        s1out.close();
	}
}