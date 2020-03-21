res = ""
%w( clang gcc9 ).each do |dir|
  Dir.chdir(dir) do
    puts( %x( cmake . 2>&1 ) )
    puts( %x( make 2>&1 ) )
    res += %x( release/sample.out )
  end
end

File.open( "list.csv", "w" ) do |f|
  f.puts( res )
end
