# bic3-vcs

##simple_message_client

###SYNOPSIS

       simple_message_client  -s server -p port -u user [-i image URL] -m mes-
       sage [-v] [-h]
       
###DESCRIPTION

       simple_message_client - Send a message to the VCS TCP/IP  message  bul-
       letin board server simple_message_server(1).

       The  message  may contain the following HTML tags: <strong>, </strong>,
       <em>, </em>, <br/>.

       For the server either the IPV4 or IPV6 address or a  hostname  must  be
       provided via the server option. The TCP port to connect to is given via
       the port option, which can either be a port number of a service name as
       listed in /etc/services.

       The  user  name  of the message submitter must be provided via the user
       option. Optionally the URL of an image for the user can be provided via
       the image URL option.

       In  case the -v commandline option is provided, execution trace message
       are written to stdout.

       Providing the -h commandline option causes a usage message to be  writ-
       ten to stdout.
       
###OPTIONS

       The following options are supported:


       -s, --server  server
              Use server (which is either an IPV4 address, an IPV6 address, or
              a hostname) as the server to connect to.


       -p, --port port
              Use port (which is either a port number or a  service  name)  as
              the TCP port to connect to. - The range for port numbers is lim-
              ited to [0..65535].


       -u, --user user
              Use user as user name for the message submission.


       -i, --image image URL
              Use image URL as URL of an image for the submitting user.


       -m, --message message
              Use message as message to submit to the bulletin board.


       -v, --verbose
              Write execution trace information to stdout

       -h, --help
              Write usage information to stdout.

##simple_message_server

###SYNOPSIS

       simple_message_server -p port [-h]
       
###DESCRIPTION

       simple_message_server - The VCS TCP/IP message bulletin board server.

       simple_message_server (VCS TCP/IP message bulletin board server) is the
       daemon program for the simple_message_client(1) client  program.   sim-
       ple_message_server  listens  on  TCP port port for incoming connections
       from  simple_message_client(1)   applications.    simple_message_server
       forks  a  new  daemon for each incoming connection, redirects stdin and
       stdout of the forked child daemons to the connected socket and executes
       the  simple_message_server_logic(1) executable in the forked child dae-
       mons.

       Providing the -h commandline option causes a usage message to be  writ-
       ten to stdout.
       
###OPTIONS

       The following options are supported:


       -p, --port port
              Use port (which must be a port number) as the TCP port to listen
              on. - The range for port numbers is limited to [0..65535].


       -h, --help
              Write usage information to stdout.