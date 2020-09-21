
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
//import java.net.ServerSocket;


public class ClientSide {

	public static void main(String[] args) throws Exception {

		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		//Scanner scan= new Scanner(System.in);
		//int sortSize;
		System.out.println("Enter the number or integers to sort: ");
		int sortSize= Integer.parseInt(br.readLine());
		//sortSize=scan.nextInt();
		int[] arr = new int[sortSize];

		System.out.println("Enter "+sortSize+ " integers for sorting: ");

		for (int i = 0; i <= arr.length - 1; i++) {

			try {
				arr[i] = Integer.parseInt(br.readLine());
			} catch (NumberFormatException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		final String ip = "localhost";
		final int port = 8165; // 0-1023 to 65535

		Socket clientSocket = new Socket(ip, port); // This block of code is to send the data
		OutputStreamWriter os = new OutputStreamWriter(clientSocket.getOutputStream());
		PrintWriter out = new PrintWriter(os);
		out.println(myToString(arr));
		os.flush();
		out.println(sortSize);
		os.flush();
		
		ServerSocket ss = new ServerSocket(9999);
		clientSocket = ss.accept();

		System.out.println("Getting data back from the sever");
		//ServerSocket serverSocket = new ServerSocket(port);
		//Socket serverReturn =serverSocket.accept();
		//br = new BufferedReader(new InputStreamReader(serverReturn.getInputStream()));
		
		br = new BufferedReader(new InputStreamReader(clientSocket.getInputStream())); // This block of code is to fetch
																						// data
		String sortedString = br.readLine();

		System.out.println(sortedString);

	}

	public static String myToString(int[] arr) {
	String result = "";

		for (int i = 0; i <= arr.length - 1; i++) {

			result = result + arr[i] + " ";
		}

		return result;

	}


}
