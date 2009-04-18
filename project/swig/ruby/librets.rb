require 'librets_native'

module Librets_native
  module Inflector
    
    extend self
    
    def camelize(lower_case_and_underscored_word, first_letter_in_uppercase = true)
      if first_letter_in_uppercase
        lower_case_and_underscored_word.to_s.gsub(/\/(.?)/) { "::" + $1.upcase }.gsub(/(^|_)(.)/) { $2.upcase }
      else
        lower_case_and_underscored_word.first + camelize(lower_case_and_underscored_word)[1..-1]
      end
    end

    def underscore(camel_cased_word)
      camel_cased_word.to_s.gsub(/::/, '/').
        gsub(/([A-Z]+)([A-Z][a-z])/,'\1_\2').
        gsub(/([a-z\d])([A-Z])/,'\1_\2').
        tr("-", "_").
        downcase
    end
  end
  
  module RubifyHelper
    def method_missing(method_id, *args)
      method_name = method_id.id2name
      # puts method_name

      # Convert xxx_id to xxx_iD so camel-case is XxxID
      method_name.gsub!("_id", "_iD")

      # Try camel-cased getter
      target_method = "Get" + Inflector.camelize(method_name)
      # puts target_method
      target_id = target_method.to_sym
      if (self.respond_to? target_id)
        return self.__send__(target_id, *args)
      end
      
      # Try camel-cased setter
      if (method_name =~ /(.*)=$/)
        target_method = "Set" + Inflector.camelize($1)
        target_id = target_method.to_sym
        if (self.respond_to? target_id)
          return self.__send__(target_id, *args)
        end
      end

      # Try camel-case without getter
      target_method = Inflector.camelize(method_name)
      # puts target_method
      target_id = target_method.to_sym
      if (self.respond_to? target_id)
        return self.__send__(target_id, *args)
      end
      
      super.method_missing(method_id, *args)
    end
  end

  class RetsSession
    include RubifyHelper
  end
  
  class LoginResponse
    include RubifyHelper
  end
  
  class CapabilityUrls
    include RubifyHelper
  end
  
  class LogoutResponse
    include RubifyHelper
  end
  
  class SearchRequest
    include RubifyHelper
  end

  class SearchResultSet
    include RubifyHelper
    
    def each
      while self.HasNext()
        yield self
      end
    end
  end
  
  class RetsMetadata
    include RubifyHelper
  end
  
  class MetadataElement
    include RubifyHelper
  end
  
  class GetObjectRequest
    include RubifyHelper
  end
  
  class GetObjectResponse
    include RubifyHelper
    
    def each_object
      while (object_descriptor = self.NextObject())
        yield object_descriptor
      end
    end
  end
  
  class ObjectDescriptor
    include RubifyHelper
    
    # Override Object#object_id
    alias_method :object_id, :GetObjectId
  end

  class RetsException
    include RubifyHelper

    # Allows ruby to print exceptions to make things easier for the
    # new user
    alias_method :to_s, :GetMessage
  end
end

module Librets
  include Librets_native
end
