# AcquRoot documentation with roothtmldoc
add_custom_target(roothtmldoc
                  COMMAND root -b -n -q htmldocU.C
                  COMMENT "Creating AcquRoot HTML documentation...")
                  