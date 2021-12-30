package org.telegram.messenger.forkgram;

import android.net.Uri;

import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.nio.charset.Charset;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class ForkTranslate {

    // Should be invoked in thread.
    public static String[] Translate(
            String fromLanguage,
            String toLanguage,
            String[] userAgents,
            CharSequence text
    ) throws JSONException {
        String userAgent = userAgents[(int) Math.round(Math.random() * (userAgents.length - 1))];

        String vqd = getVqd(userAgent);
        Uri uri = Uri.parse("https://duckduckgo.com/translation.js")
                .buildUpon()
                .appendQueryParameter("vqd", vqd)
                .appendQueryParameter("query", "translate")
                .appendQueryParameter("to", toLanguage)
                .build();

        String response = fetchTranslate(uri.toString(), userAgent, text.toString());
        JSONTokener tokener = new JSONTokener(response);
        JSONObject obj = new JSONObject(tokener);
        String source = obj.getString("detected_language");
        String result = obj.getString("translated");
        return new String[]{result, source};
    }

    private static String readResponse(HttpURLConnection connection) {
        StringBuilder textBuilder = new StringBuilder();
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(connection.getInputStream(), Charset.forName("UTF-8")));
            try {
                int c;
                while ((c = reader.read()) != -1) {
                    textBuilder.append((char) c);
                }
            } finally {
                reader.close();
            }
        } catch (Exception ignore) {
        }
        return textBuilder.toString();
    }

    private static String getVqd(String userAgent) {
        try {
            URL url = new URL("https://duckduckgo.com/?q=translate&ia=web");
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("GET");
            connection.setRequestProperty("User-Agent", userAgent);
            String response = readResponse(connection);
            int start = response.indexOf("vqd=");
            int end = response.indexOf(";", start);
            String substring = response.substring(start + "vqd=".length(), end);
            Pattern pattern = Pattern.compile("[0-9-]+");
            Matcher matcher = pattern.matcher(substring);
            matcher.find();
            return matcher.group(0);
        } catch (Exception e) {
            return "";
        }
    }

    private static String fetchTranslate(String uri, String userAgent, String text) {
        try {
            URL url = new URL(uri);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            connection.setRequestMethod("POST");
            connection.setRequestProperty("User-Agent", userAgent);
            connection.setRequestProperty("Content-Type", "application/json");
            connection.setDoOutput(true);
            OutputStream os = connection.getOutputStream();
            try {
                os.write(text.getBytes());
                os.flush();
            } finally {
                os.close();
            }
            return readResponse(connection);
        } catch (Exception e) {
            return "";
        }
    }
}
