

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.NoSuchElementException;
import java.util.StringTokenizer;
public class ServerSide {
    public static void main(String[] args) throws Exception {
        // TODO Auto-generated method stub
        System.out.println("The server is started");
        final int port = 8165;
        ServerSocket serverSocket = new ServerSocket(port);
        System.out.println("The server is waiting for client request");
        Socket s = serverSocket.accept();
        System.out.println("The client is connected");
        BufferedReader br = new BufferedReader(new InputStreamReader(s.getInputStream())); // This block of code is to
                                                                                            // fetch data
        String str = br.readLine();
        //serverSocket.accept();
        int sortSize= Integer.parseInt(br.readLine());
        
        System.out.println("The client data has been received");
        int[] arr = new int[sortSize];
        StringTokenizer stringToken = new StringTokenizer(str);
        for (int i = 0; i <= str.length() - 1; i++) {
            try {
                arr[i] = Integer.parseInt(stringToken.nextToken());
            } catch (NoSuchElementException e) {
            } catch (Exception e) {
            }
        }
        bubbleSort(arr);
        s = new Socket("localhost", 9999);
        OutputStreamWriter os = new OutputStreamWriter(s.getOutputStream()); // This block of code is to send the data
        PrintWriter out = new PrintWriter(os);
        out.println(myToString(arr));
        os.flush();
    }
    public static void bubbleSort(int[] array) {
        boolean sorted = false;
        int temp;
        while (!sorted) {
            sorted = true;
            for (int i = 0; i < array.length - 1; i++) {
                if (array[i] > array[i + 1]) {
                    temp = array[i];
                    array[i] = array[i + 1];
                    array[i + 1] = temp;
                    sorted = false;
                }
            }
        }
    }
    public static String myToString(int[] arr) {
        String result = "";
        for (int i = 0; i <= arr.length - 1; i++) {
            result = result + arr[i] + " ";
        }
        return result;
    }
}