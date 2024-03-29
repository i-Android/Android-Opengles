package com.alibaba.android.babylon.graphic;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.content.Context;
import android.util.Xml;

public class LWGLAssetsUtil2 {
	public static class Entry {
	    public final String title;
	    public final String link;
	    public final String summary;

	    private Entry(String title, String summary, String link) {
	        this.title = title;
	        this.summary = summary;
	        this.link = link;
	    }
	}
	
	 // We don't use namespaces
    private static final String ns = null;
    
	// Parses the contents of an entry. If it encounters a title, summary, or link tag, hands them off
	// to their respective "read" methods for processing. Otherwise, skips the tag.
	private static Entry readEntry(XmlPullParser parser) throws XmlPullParserException, IOException {
	    parser.require(XmlPullParser.START_TAG, ns, "entry");
	    String title = null;
	    String summary = null;
	    String link = null;
	    while (parser.next() != XmlPullParser.END_TAG) {
	        if (parser.getEventType() != XmlPullParser.START_TAG) {
	            continue;
	        }
	        String name = parser.getName();
	        if (name.equals("title")) {
	            title = readTitle(parser);
	        } else if (name.equals("summary")) {
	            summary = readSummary(parser);
	        } else if (name.equals("link")) {
	            link = readLink(parser);
	        } else {
	            skip(parser);
	        }
	    }
	    return new Entry(title, summary, link);
	}
	
	// Processes title tags in the feed.
	private static String readTitle(XmlPullParser parser) throws IOException, XmlPullParserException {
	    parser.require(XmlPullParser.START_TAG, ns, "title");
	    String title = readText(parser);
	    parser.require(XmlPullParser.END_TAG, ns, "title");
	    return title;
	}
	  
	// Processes link tags in the feed.
	private static String readLink(XmlPullParser parser) throws IOException, XmlPullParserException {
	    String link = "";
	    parser.require(XmlPullParser.START_TAG, ns, "link");
	    String tag = parser.getName();
	    String relType = parser.getAttributeValue(null, "rel");  
	    if (tag.equals("link")) {
	        if (relType.equals("alternate")){
	            link = parser.getAttributeValue(null, "href");
	            parser.nextTag();
	        } 
	    }
	    parser.require(XmlPullParser.END_TAG, ns, "link");
	    return link;
	}

	// Processes summary tags in the feed.
	private static String readSummary(XmlPullParser parser) throws IOException, XmlPullParserException {
	    parser.require(XmlPullParser.START_TAG, ns, "summary");
	    String summary = readText(parser);
	    parser.require(XmlPullParser.END_TAG, ns, "summary");
	    return summary;
	}

	// For the tags title and summary, extracts their text values.
	private static String readText(XmlPullParser parser) throws IOException, XmlPullParserException {
	    String result = "";
	    if (parser.next() == XmlPullParser.TEXT) {
	        result = parser.getText();
	        parser.nextTag();
	    }
	    return result;
	}
	
	private static void skip(XmlPullParser parser) throws XmlPullParserException, IOException {
		if (parser.getEventType() != XmlPullParser.START_TAG) {
			throw new IllegalStateException();
		}
		int depth = 1;
		while (depth != 0) {
			switch (parser.next()) {
			case XmlPullParser.END_TAG:
				depth--;
				break;
			case XmlPullParser.START_TAG:
				depth++;
				break;
			}
		}
	}
	
	public static String getAssetsShader(Context context, String shaderName) {
		
		String shaderContent = null;
		InputStream inputStream = null;
		List entries = new ArrayList();
		try {
			inputStream = context.getAssets().open("filter/"+shaderName+"/"+shaderName+".xml");
//			int size = inputStream.available();
//			byte[] buffer = new byte[size];
//			inputStream.read(buffer);
//			inputStream.close();
//			shaderContent = new String(buffer);
			
			
			XmlPullParser parser = Xml.newPullParser();
			parser.setFeature(XmlPullParser.FEATURE_PROCESS_NAMESPACES, false);
			parser.setInput(inputStream, null);
			parser.nextTag();
			
			

			parser.require(XmlPullParser.START_TAG, ns, "feed");
			while (parser.next() != XmlPullParser.END_TAG) {
				if (parser.getEventType() != XmlPullParser.START_TAG) {
					continue;
				}
				String name = parser.getName();
				// Starts by looking for the entry tag
				if (name.equals("entry")) {
					entries.add(readEntry(parser));
				} else {
					skip(parser);
				}
			}  
			    
			
		} catch (IOException e) {
			e.printStackTrace();
		} catch (XmlPullParserException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} finally {
			try {
				inputStream.close();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		return shaderContent;
	}



}
