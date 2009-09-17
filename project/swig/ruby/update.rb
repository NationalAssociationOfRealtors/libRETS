#!/usr/bin/env ruby

require 'librets'

include Librets

begin
    session = RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

    if !session.login("Joe", "Schmoe")
      puts "Invalid login"
      exit 1
    end

    request = session.create_update_request("Property", "RES")
    request.delimiter = "|"
    request.SetField("ListingID", "LN000005")
    request.SetField("CloseDate", "2009-08-20T00:00:00")
    request.update_type = "Change"
    request.validate_flag = UpdateRequest::VALIDATE_ONLY

    results = session.update(request)

    columns = results.columns
    results.each do |result|
      columns.each do |column|
        puts column + ": " + result.string(column)
      end
      puts
    end

    results.each_error do |result|
        puts result.error_field_name    \
             + ", Error: "              \
             + result.error_number.to_s \
             + " at offset "            \
             + result.error_offset.to_s \
             + ", Message: "            \
             + result.error_text        \
    end

    results.each_warning do |result|
        puts result.warning_field_name          \
             + ", Error: "                      \
             + result.warning_number.to_s       \
             + " at offset "                    \
             + result.warning_offset.to_s       \
             + ", Message: "                    \
             + result.warning_text              \
             + ", Response Required: "          \
             + result.warning_response_required
    end

    logout = session.logout

    puts "Billing info: " + logout.billing_info
    puts "Logout message: " + logout.logout_message
    puts "Connect time: " + logout.connect_time.to_s
rescue RetsException => e
    puts e
end
