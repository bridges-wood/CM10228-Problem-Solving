import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.regex.Pattern;

/**
 * @author Anonymous (do not change)
 *
 *         Question 6:
 *
 *         Implement in Java a similar algorithm to that in Q5, i.e. given a
 *         block of text your algorithm should be able to redact words from a
 *         given set and outputs the result to a file called â€œresult.txtâ€�.
 *         However, in this implementation of the algorithm all redactable words
 *         will be proper nouns (i.e. a name used for an individual person,
 *         place, or organisation, spelled with an initial capital letter) and
 *         your algorithm should take into account that the list of redactable
 *         words might not be complete. For example, given the block of text: It
 *         was in July, 1805, and the speaker was the well-known Anna Pavlovna
 *         Scherer, maid of honor and favorite of the Empress Marya Fedorovna.
 *         With these words she greeted Prince Vasili Kuragin, a man of high
 *         rank and importance, who was the first to arrive at her reception.
 *         Anna Pavlovna had had a cough for some days. She was, as she said,
 *         suffering from la grippe; grippe being then a new word in St.
 *         Petersburg, used only by the elite.
 *
 *         and the redactable set of words Anna Pavlovna Scherer, St.
 *         Petersburg, Marya Fedorovna
 *
 *         the output text should be It was in ****, 1805, and the speaker was
 *         the well-known **** ******** *******, maid of honor and favorite of
 *         the ******* ***** *********. With these words she greeted ******
 *         ****** *******, a man of high rank and importance, who was the first
 *         to arrive at her reception. **** ******** had had a cough for some
 *         days. She was, as she said, suffering from la grippe; grippe being
 *         then a new word in *** **********, used only by the elite.
 *
 *         You should test your program using the example files provided.
 */

public class CWK2Q6 {
	/** File to write the result to. */
	public static final String RESULT = "./result.txt";
	/** Regex to match sentence terminating characters. */
	public static final Pattern PUNCTUATION = Pattern.compile("[.!?\"*]");
	/** List of all values that would be identified as proper nouns, but are not. */
	public static final ArrayList<String> falsePositives = new ArrayList<String>(
			Arrays.asList(new String[] { "I", "A", "I'll", "I've", "I'm", "I'd", "BOOK", "CHAPTER", "II", "III", "IV", "V",
					"VI", "VII", "VIII", "IX", "X", "XI", "XII", "XIII", "XIV", "XV", "XVI", "XVII", "XVIII", "XIX", "XX", "XXI",
					"XXII", "XXIII", "XXIV", "XXV", "XXVI", "XXVII", "XXVIII", "XXIX", "XXX", "XXXI", "XXXII", "XXXIII", "XXXIV",
					"XXXV", "XXXVI", "XXXVII", "XXXVIII", "XXXIX", "ONE", "TWO", "THREE", "FOUR", "FIVE", "SIX", "SEVEN", "EIGHT",
					"NINE", "TEN", "ELEVEN", "TWELVE", "THIRTEEN", "FOURTEEN", "FIFTEEN", "FIRST", "SECOND", "EPILOGUE" }));
	/** List to keep track of all proper nouns to be redacted. */
	public static ArrayList<String> redactables;

	public static void main(String[] args) {
		String inputFile = "./warandpeace.txt";
		String redactFile = "./redact.txt";
		redactWords(inputFile, redactFile);
	}

	/**
	 * Redacts all proper nouns from one file, using another file as the initial
	 * basis.
	 * 
	 * @param textFilename        The filename of the file to have all proper nouns
	 *                            redacted from.
	 * @param redactWordsFilename The filename of the file containing the initial
	 *                            basis of proper nouns.
	 */
	public static void redactWords(String textFilename, String redactWordsFilename) {
		try (BufferedReader br = new BufferedReader(new FileReader(textFilename))) {
			initialiseRedactables(redactWordsFilename);

			// Generate string from file.
			StringBuilder text = new StringBuilder();
			String line;
			while ((line = br.readLine()) != null) {
				text.append(line + " ");
			}

			System.out.println(redactables);

			// Write result
			String redacted = redactText(text.toString());
			System.out.println(redactables);
			writeResult(redacted);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	/**
	 * Adds all words in a text file to the list of redactable words.
	 * 
	 * @param textFilename The filename of the file containing the redactable words.
	 * @throws FileNotFoundException If the file does not exist.
	 */
	public static void initialiseRedactables(String textFilename) throws FileNotFoundException {
		try (BufferedReader br = new BufferedReader(new FileReader(textFilename))) {
			redactables = new ArrayList<String>();
			String line;
			while ((line = br.readLine()) != null) {
				String[] words = line.split(" ");
				for (String word : words) {
					addRedactable(word);
				}
			}
		} catch (Exception e) {
			throw new FileNotFoundException("File " + textFilename + " not found.");
		}
	}

	/**
	 * Redacts all of the proper nouns from a string.
	 * 
	 * @param text The string to be redacted.
	 * @return The redacted version of the string.
	 */
	public static String redactText(String text) {
		String[] words = text.split("[\\s-—]"); // Splits on word boundaries.

		// Pass 1, find all proper nouns.
		for (int i = 1; i < words.length; i++) {
			String previous = words[i - 1];
			String current = words[i];
			String cleanedCurrent = current.replaceAll("[^A-Za-z']+", "");
			if (PUNCTUATION.matcher(previous).find()) { // If the previous word ends with a punctuation mark, ignore it.
				i++;
				continue;
			}

			if (isProperNoun(cleanedCurrent) && (!current.startsWith("\"") && !current.startsWith("'"))) {
				addRedactable(cleanedCurrent); // Identify proper nouns.
			}
		}

		// Pass 2, replace all occurences of proper nouns found in pass 1.
		for (int i = 0; i < words.length; i++) {
			// This method redacts over-zealously due to the complexity of properly
			// identifying proper nouns in compound forms, e.g. "Her Majesty"
			String current = words[i];
			String cleanedCurrent = current.replaceAll("[^A-Za-z']+", "");
			if (redactables.contains(cleanedCurrent)) {
				words[i] = redactWord(current);
			}
		}

		return String.join(" ", words); // Reconstruct input.
	}

	/**
	 * Determines whether or not a string is a proper noun.
	 * 
	 * @param word The string to be checked.
	 * @return true if the string is a proper noun, false otherwise.
	 */
	public static boolean isProperNoun(String word) {
		if (falsePositives.contains(word))
			return false;
		for (char letter : word.toCharArray()) {
			if (Character.isUpperCase(letter)) {
				return true;
			}
		}
		return false;
	}

	/**
	 * Adds a word to the list of redactable words.
	 * 
	 * @param word The word to be added.
	 */
	public static void addRedactable(String word) {
		if (redactables.contains(word))
			return;
		redactables.add(word);
	}

	/**
	 * Replaces all alphabetic characters and hyphens with *s in a string.
	 * 
	 * @param word The string to be redacted.
	 * @return The redacted string.
	 */
	public static String redactWord(String word) {
		return word.replaceAll("[a-zA-Z-]", "*");
	}

	/**
	 * Writes a string to the RESULT file.
	 * 
	 * @param redactedText The string to be written to the RESULT file.
	 */
	public static void writeResult(String redactedText) {
		try (BufferedWriter bw = new BufferedWriter(new FileWriter(RESULT))) {
			bw.write(redactedText);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
