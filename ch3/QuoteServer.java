/**
 * Time-of-day server listening to port 6013.
 *
 * Figure 3.27
 *
 * @author Silberschatz, Gagne, and Galvin. 
 * Operating System Concepts  - Tenth Edition
 * Copyright John Wiley & Sons - 2018.
 */
 
import java.net.*;
import java.util.Random;
import java.io.*;

public class QuoteServer
{
	public static void main(String[] args)  {
		try {
			ServerSocket sock = new ServerSocket(6017);

			// now listen for connections
			while (true) {
				Socket client = sock.accept();
				// we have a connection
				
				PrintWriter pout = new PrintWriter(client.getOutputStream(), true);
				// write the Date to the socket
				pout.println(quoteOfTheDay());

				// close the socket and resume listening for more connections
				client.close();
			}
		}
		catch (IOException ioe) {
				System.err.println(ioe);
		}
	}

	private static String quoteOfTheDay() {
		Random random = new Random();
		int idx = random.nextInt(3);
	
		return QUOTES[idx];
	}

    private static String[] QUOTES = {"Next Friday will not be your lucky day.  As a matter of fact, you don't	have a lucky day this year.","Good day to deal with people in high places; particularly lonely stewardesses.", "Every cloud engenders not a storm.\n\t\t	-- William Shakespeare, Henry VI"};
}
