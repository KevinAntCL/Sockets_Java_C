// KEVIN ANTONIO CHÁVEZ LARA - 185380
import java.io.*;
import java.net.*;

class MovieServer {
    public static void main(String args[]) throws IOException 
    {
        if (args.length < 1)
        { 
            System.out.println("Modo de uso: MovieClient portNumber");
            System.exit(1);
        }
        // Register service on port
        int port = Integer.parseInt(args[0]);   
        ServerSocket s = new ServerSocket(port);
        Socket s1=s.accept(); // Wait and accept a connection
        // Get a communication stream associated with the socket
        InputStream s1In = s1.getInputStream();
        DataInputStream dis = new DataInputStream(s1In);
        byte[] buffer = new byte[255];
        dis.read(buffer);
        String mensajeProtocolo = new String (buffer);
        String[] mensajeSplit = mensajeProtocolo.split(",");
    
        // Abrimos el archivo CSV para empezar la busqueda:
        int resultadoProtocolo = 0;
        String resultadoBusqueda = null;
        try
        {
        BufferedReader reader = new BufferedReader(new FileReader("movies.csv"));
        String line = null;
            while ((line = reader.readLine()) != null) 
            {
                String[] partes = line.split(",");       
                int result = mensajeSplit[1].compareTo(partes[0]);
                System.out.println(partes[0]);
                System.out.println(result);

                if (result > 200)
                {
                    resultadoProtocolo = 1;
                    resultadoBusqueda = partes[Integer.parseInt(mensajeSplit[0])];
                }
                else if (resultadoProtocolo == 0)
                {
                    resultadoProtocolo = 0;
                }
            }
            resultadoBusqueda = resultadoProtocolo + "," + resultadoBusqueda;
        }
        catch (IOException e) {
                e.printStackTrace(); 
        }

        // Mandamos de vuelta el resultado de la busqueda al cliente:
        OutputStream s1out = s1.getOutputStream();
        DataOutputStream dos = new DataOutputStream (s1out);
        dos.write(resultadoBusqueda.getBytes("utf-8"));
    
        // Close the connection, but not the server socket
        dis.close();
        s1In.close();
        s1.close();
        dos.close();
        s1out.close();
    }
}