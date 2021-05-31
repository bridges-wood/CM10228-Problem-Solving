import java.util.ArrayList;

/**
 * @author Anonymous (do not change)
 *
 *         Question 1:
 *
 *         Implement the Shellsort algorithm
 *         (https://en.wikipedia.org/wiki/Shellsort) for an array of up to 1000
 *         signed doubles in Java. Your solution must use concrete gaps of [1,
 *         3, 7, 15, 31, 63, 127, 255, 511]. Your solution must print the
 *         (partially) sorted array after each gap on a new line in the form:
 *         [a0, a1, a2, a3, ..., an] Where an is the nth element in the
 *         (partially) sorted array (please note the space after the commas),
 *         and each element should be formatted to 2 decimal places (e.g. 1.00).
 *
 */

public class CWK2Q1 {
	private static final int[] gaps = { 511, 255, 127, 63, 31, 15, 7, 3, 1 }; // Gaps in decreasing size.

	public static void main(String[] args) {
		ArrayList<Double> testList = new ArrayList<Double>();
		testList.add(3.4);
		testList.add(6.55);
		testList.add(-12.2);
		testList.add(1.73);
		testList.add(140.98);
		testList.add(-4.18);
		testList.add(52.87);
		testList.add(99.14);
		testList.add(73.202);
		testList.add(-23.6);

		shell_sort(testList);
	}

	/**
	 * Sorts and prints an {@link ArrayList} of doubles using the
	 * <a href="https://en.wikipedia.org/wiki/Shellsort">Shellsort Algorithm</a>.
	 * 
	 * @param array The list of doubles to be sorted.
	 */
	private static void shell_sort(ArrayList<Double> array) {
		for (int gap : gaps) {
			for (int i = gap; i < array.size(); i++) {
				int j = i;
				Double temp = array.get(i);
				while (j >= gap && array.get(j - gap) > temp) {
					array.set(j, array.get(j - gap));
					j -= gap;
				}
				array.set(j, temp);
			}
			printArr(array);
		}
	}

	/**
	 * Prints an {@link ArrayList} of doubles.
	 * 
	 * @param array The list of doubles to print.
	 */
	private static void printArr(ArrayList<Double> array) {
		StringBuilder sb = new StringBuilder();
		sb.append('[');
		for (Double d : array) {
			sb.append(String.format("%.2f", d) + ", ");
		}
		sb.replace(sb.length() - 2, sb.length(), "]");
		System.out.println(sb.toString());
	}
}
