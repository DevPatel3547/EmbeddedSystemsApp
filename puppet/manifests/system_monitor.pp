node 'embedded_device' {
  # Assume the system_monitor binary has been copied to /opt/system_monitor
  file { '/opt/system_monitor':
    ensure => directory,
    owner  => 'root',
    group  => 'root',
    mode   => '0755',
  }

  file { '/opt/system_monitor/system_monitor':
    ensure => file,
    mode   => '0755',
    source => 'puppet:///modules/system_monitor/system_monitor',  # Adjust the source as needed
  }

  # Create an init script (or systemd unit) for the daemon.
  file { '/etc/systemd/system/system_monitor.service':
    ensure  => file,
    content => "[Unit]
Description=Embedded System Monitor Daemon

[Service]
ExecStart=/opt/system_monitor/system_monitor
Restart=always

[Install]
WantedBy=multi-user.target
",
    mode => '0644',
  }

  # Enable and start the service
  service { 'system_monitor':
    ensure    => running,
    enable    => true,
    subscribe => File['/etc/systemd/system/system_monitor.service'],
  }
}
