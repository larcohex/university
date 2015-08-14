import java.io.IOException;
import java.util.StringTokenizer;
import java.util.ArrayList;
import java.io.File;
import java.io.DataInput;
import java.io.DataOutput;


import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.io.WritableComparable;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;
import org.apache.hadoop.mapreduce.lib.chain.ChainReducer;
import org.apache.hadoop.mapreduce.lib.output.MultipleOutputs;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;

public class WSH
{
	private static int k;

	public static boolean delete (File f)
	{
		if (f.exists())
		{
			File[] files = f.listFiles();
			if (files != null)
			{
				for (int i = 0; i < files.length; ++i)
				{
					files[i].delete();
				}
			}
		}
		return f.delete();
	}

	public static class Pair implements WritableComparable <Pair>
	{
		private String first;
		private String second;

		public Pair ()
		{
		}


		public Pair (String s1, String s2)
		{
			this.first = s1;
			this.second = s2;
		}

		public String getFirst()
		{
			return first;
		}


		public String getSecond()
		{
			return second;
		}

		@Override
		public void readFields (DataInput in) throws IOException
		{
			first = in.readUTF();
			second = in.readUTF();
		}


		@Override
		public void write(DataOutput out) throws IOException
		{
			out.writeUTF (first);
			out.writeUTF (second);
		}


		@Override
		public int compareTo (Pair t)
		{
			int cmp = this.first.compareTo (t.first); 
			if (cmp != 0)
			{
				return cmp;
			}
			return this.second.compareTo (t.second);
		}


		@Override
		public boolean equals (Object obj)
		{
			if (obj == null)
			{
				return false;
			}
			if (this.getClass() != obj.getClass())
			{
				return false;
			}
			if (!(obj instanceof Pair))
			{
				return false;
			}
			final Pair temp = (Pair) obj;
			if (this.first != temp.first && (this.first == null || !this.first.equals (temp.first)))
			{
				return false;
			}
			if (this.second != temp.second && (this.second == null || !this.second.equals (temp.second)))
			{
				return false;
			}
			return true;
		}


		@Override
		public int hashCode()
		{
			return 31 * (this.first != null ? this.first.hashCode() : 0) + (this.second != null ? this.second.hashCode() : 0);
		}
	}


	public static class PairCreatorMapper extends Mapper <Object, Text, Pair, IntWritable>
	{
		private final static IntWritable one = new IntWritable(1);
		public void map (Object key, Text value, Context context) throws IOException, InterruptedException
		{
			StringTokenizer itr = new StringTokenizer (value.toString());
			ArrayList <String> words = new ArrayList <String>();
			String lineID = itr.nextToken();
			while (itr.hasMoreTokens())
			{
				String newWord = itr.nextToken();
				words.add (newWord);
			}
			for (int i = 0; i < words.size() - 1; ++i)
			{
				for (int j = i + 1; j < words.size(); ++j)
				{
					if (words.get(i).equals(words.get(j)))
					{
						words.remove(j);
						--j;
					}
				}
			}
			for (int i = 0; i < words.size(); ++i)
			{
				for (int j = 0; j < words.size() - i - 1; ++j)
				{
					String s1 = words.get (j);
					String s2 = words.get (j + 1);
					int compare = s1.compareTo (s2);
					if (compare > 0)
					{
						words.set (j, s2);
						words.set (j + 1, s1);
					}
				}
			}
			for (int i = 0; i < words.size(); ++i)
			{
				String word1 = words.get (i);
				for (int j = i + 1; j < words.size(); ++j)
				{
					String word2 = words.get (j);
					Pair newPair = new Pair (word1, word2);
					context.write (newPair, one);
				}
			}
		}
	}



	public static class PairCreatorReducer extends Reducer <Pair, IntWritable, Text, IntWritable>
	{
		private IntWritable result = new IntWritable();
		private final static IntWritable one = new IntWritable (1);
		public void reduce (Pair key, Iterable <IntWritable> values, Context context) throws IOException, InterruptedException
		{
			int sum = 0;
			for (IntWritable val : values)
			{
				sum += val.get();
			}
			result.set (sum);
			Text newKey = new Text();
			String keyTemp = key.getFirst() + " " + key.getSecond();
			newKey.set (keyTemp);
			context.write (newKey, result);
		}
	}



	public static class WSHMapper extends Mapper <Object, Text, IntWritable, IntWritable>
	{
		private final static IntWritable one = new IntWritable(1);
		public void map (Object key, Text value, Context context) throws IOException, InterruptedException
		{
			ArrayList <String> sums = new ArrayList <String>();
			String inp = value.toString();
			String[] lines = inp.split ("\n");
			for (int i = 0; i < lines.length; ++i)
			{
				String[] parts = lines[i].split ("\t");
				sums.add (parts[1]);
			}
			for (int i = 0; i < sums.size(); ++i)
			{
				int tempInt = Integer.parseInt (sums.get (i));
				IntWritable newSum = new IntWritable();
				newSum.set (tempInt);
				context.write (newSum, one);
			}
		}
	}


	public static class WSHReducer extends Reducer <IntWritable, IntWritable, IntWritable, IntWritable>
	{
		private IntWritable result = new IntWritable();
		private final static IntWritable one = new IntWritable (1);
		public void reduce (IntWritable key, Iterable <IntWritable> values, Context context) throws IOException, InterruptedException
		{
			int sum = 0;
			for (IntWritable val : values)
			{
				sum += val.get();
			}
			result.set (sum);
			context.write (key, result);
		}
	}

	public static class SWReducer extends Reducer <Pair, IntWritable, Text, IntWritable>
	{
		private IntWritable result = new IntWritable();
		private Text pair = new Text();
		public void reduce (Pair key, Iterable <IntWritable> values, Context context) throws IOException, InterruptedException
		{
			int sum = 0;
			for (IntWritable val : values)
			{
				sum += val.get();
			}
			String newPair = "(" + key.getFirst() + "," + key.getSecond() + ")";
			pair.set (newPair);
			result.set (sum);
			if (sum >= k)
			{
				context.write (pair, result);
			}
		}
	}
  

	public static void main (String[] args) throws Exception
	{
		Configuration conf = new Configuration();
		String mode = args[0];
		if (mode.equals ("sim"))
		{
			k = Integer.parseInt (args[1]);
			Job sw = Job.getInstance (conf, "SW");
			sw.setJarByClass (WSH.class);
			sw.setMapperClass (PairCreatorMapper.class);
			sw.setReducerClass (SWReducer.class);
			sw.setMapOutputKeyClass (Pair.class);
			sw.setMapOutputValueClass (IntWritable.class);
			sw.setOutputKeyClass (Text.class);
			sw.setOutputValueClass (IntWritable.class);
			FileInputFormat.addInputPath (sw, new Path (args[2]));
			FileOutputFormat.setOutputPath (sw, new Path (args[3]));
			System.exit (sw.waitForCompletion (true) ? 0 : 1);
		}
		else if (mode.equals ("hist"))
		{
			Job pairCreation = Job.getInstance (conf, "PairCreation");
			pairCreation.setJarByClass (WSH.class);
			pairCreation.setMapperClass (PairCreatorMapper.class);
			pairCreation.setReducerClass (PairCreatorReducer.class);
			pairCreation.setMapOutputKeyClass (Pair.class);
			pairCreation.setMapOutputValueClass (IntWritable.class);
			pairCreation.setOutputKeyClass (IntWritable.class);
			pairCreation.setOutputValueClass (IntWritable.class);
			FileInputFormat.addInputPath (pairCreation, new Path (args[1]));
			FileOutputFormat.setOutputPath (pairCreation, new Path ("t"));
			Job wsh = Job.getInstance (conf, "WSH");
			wsh.setJarByClass (WSH.class);
			wsh.setMapperClass (WSHMapper.class);
			wsh.setReducerClass (WSHReducer.class);
			wsh.setMapOutputKeyClass (IntWritable.class);
			wsh.setMapOutputValueClass (IntWritable.class);
			wsh.setOutputKeyClass (IntWritable.class);
			wsh.setOutputValueClass (IntWritable.class);
			FileInputFormat.addInputPath (wsh, new Path ("t"));
			FileOutputFormat.setOutputPath (wsh, new Path (args[2]));
			if (pairCreation.waitForCompletion (true))
			{
				boolean tempb = wsh.waitForCompletion (true);
				if (tempb)
				{
					File f = new File ("t");
					delete (f);
					System.exit (0);
				}
			}
			else
			{
				System.exit (1);
			}
		}
	}
}
