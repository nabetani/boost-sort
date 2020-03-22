res = ""
%w( clang gcc9 ).each do |dir|
  Dir.chdir(dir) do
    puts( %x( cmake . 2>&1 ) )
    puts( %x( make 2>&1 ) )
    (4..7).each do |ix|
      cmd = "release/sample.out #{10**ix}"
      res += %x(#{cmd})
      $stderr.puts( cmd )
    end
  end
end

File.open( "list.csv", "w" ) do |f|
  f.puts( res )
end
