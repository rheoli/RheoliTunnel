#!/usr/bin/env ruby
#=
#= Rheoli Tunnel Client-Server
#=

require 'eventmachine'

class RTunnel < EM::Connection

  def post_init
  end

  def receive_data(_data)
    puts [:receive_data].inspect
    end
  end

  def unbind
  end

end

EM.run do
  s=EM.start_server 'localhost', 2222, RTunnel
end

