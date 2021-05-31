import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * @author Anonymous (do not change)
 * 
 *         Question 8:
 * 
 *         You are given the following information, but you may prefer to do
 *         some research for yourself. • 1 Jan 1900 was a Monday. • Thirty days
 *         has September, April, June and November. All the rest have
 *         thirty-one, saving February alone, which has twenty-eight, rain or
 *         shine. And on leap years, twenty-nine. • A leap year occurs on any
 *         year evenly divisible by 4, but not on a century unless it is
 *         divisible by 400.
 * 
 *         How many Tuesdays fell on the first of the month during the twentieth
 *         century (1 Jan 1901 to 31 Dec 2000)?
 * 
 *         Note, this problem is inspired by Project Euler so, as stated in the
 *         rules of Project Euler, your solution should return an answer under
 *         60 seconds.
 */

public class CWK2Q8 {
	private static final SimpleDateFormat formatter = new SimpleDateFormat("dd/MM/yyyy");
	private static final String LAST_DATE = "31/12/2000";

	/**
	 * Counts the number of times Tuesday falls on the first of the month from
	 * 1/1/1901 to 31/21/2000.
	 */
	public static int howManyTuesdays() {
		Calendar cal = Calendar.getInstance();
		cal.set(1901, 0, 1); // Initialise calendar

		int tuesdaysOnTheFirst = 0;
		while (!isFinalDate(cal)) {
			if (cal.get(Calendar.DAY_OF_WEEK) == Calendar.TUESDAY && cal.get(Calendar.DAY_OF_MONTH) == 1)
				tuesdaysOnTheFirst++;

			cal.add(Calendar.DATE, 1);
		}

		return tuesdaysOnTheFirst;
	}

	/**
	 * Checks if a calendar instance matches the specified final date.
	 * 
	 * @param cal The calendar instance to be checked.
	 * @return True if the calendar matches the specified date, false otherwise.
	 */
	public static boolean isFinalDate(Calendar cal) {
		return formatter.format(cal.getTime()).equals(LAST_DATE);
	}

	public static void main(String[] args) {
		int result = CWK2Q8.howManyTuesdays();
		System.out.println("Number of Tuesdays = " + result);
	}
}
