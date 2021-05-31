import java.util.ArrayList;
import java.util.Collections;

/**
 * @author Anonymous (do not change)
 *
 *         Question 2:
 *
 *         Implement interpolation search for a list of Strings in Java using
 *         the skeleton class provided. The method should return the position in
 *         the array if the string is present, or -1 if it is not present.
 */

public class CWK2Q2 {

	public static void main(String[] args) {
		ArrayList<String> testList = new ArrayList<String>();
		testList.add("Hello");
		testList.add("World");
		testList.add("How");
		testList.add("Are");
		testList.add("You");

		int result = interpolation_search(testList, "You");
		System.out.println("Result = " + result);
	}

	/**
	 * Searches an {@link ArrayList} of Strings for a given item using
	 * <a href="https://en.wikipedia.org/wiki/Interpolation_search">Interpolation
	 * Search</a>.
	 * 
	 * @param array The list of strings to be searched.
	 * @param item  The item to be searched for.
	 * @return The index of the item in the list, if it exists or -1 if it is does
	 *         not.
	 */
	private static int interpolation_search(ArrayList<String> array, String item) {
		ArrayList<IndexTracker> tracked = createIndexMappings(array);
		Collections.sort(tracked); // Pre-sorts strings for later use by interpolation search.

		int low = 0;
		int high = tracked.size() - 1;
		int mid;

		while (!tracked.get(high).equals(tracked.get(low)) && tracked.get(low).value.compareTo(item) <= 0
				&& tracked.get(high).value.compareTo(item) >= 0) {
			mid = low
					+ ((item.compareTo(tracked.get(low).value)) * (high - low) / tracked.get(high).compareTo(tracked.get(low)));
			// As Strings are {@link Comparable} the different between two Strings can be
			// used for interpolation.

			if (tracked.get(mid).value.compareTo(item) < 0) {
				low = mid + 1;
			} else if (tracked.get(mid).value.compareTo(item) > 0) {
				high = mid - 1;
			} else {
				return mid;
			}
		}

		if (item.equals(tracked.get(low).value)) {
			return tracked.get(low).originalIndex; // Extract original index from found item.
		} else {
			return -1;
		}
	}

	/**
	 * Converts an {@link ArrayList} of Strings to an ArrayList of index trackers.
	 * 
	 * @param array The array to be converted.
	 * @return An arrayList of index trackers.
	 */
	private static ArrayList<IndexTracker> createIndexMappings(ArrayList<String> array) {
		ArrayList<IndexTracker> trackedArray = new ArrayList<IndexTracker>();
		for (int i = 0; i < array.size(); i++) {
			trackedArray.add(i, new CWK2Q2.IndexTracker(i, array.get(i)));
		}
		return trackedArray;
	}

	/**
	 * Class to handle tracking of the indices pre-sorting.
	 */
	private static class IndexTracker implements Comparable<IndexTracker> {
		public int originalIndex;
		public String value;

		public IndexTracker(int index, String value) {
			this.originalIndex = index;
			this.value = value;
		}

		@Override
		public int compareTo(CWK2Q2.IndexTracker o) {
			return this.value.compareTo(o.value); // Compares based on the stored string value.
		}

	}
}
